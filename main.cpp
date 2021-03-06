/*
 * Note: this file originally auto-generated by mib2c using
 *       version $ of $ 
 */
/*
 * standard Net-SNMP includes 
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <signal.h>
#include <iostream>
#include "netSnmpMQTTTable.h"
#include "TMQTTSNMPHandler.h"



static int keep_running;

static RETSIGTYPE stop_server(int a) {
	keep_running = 0;
}



static void usage(void) {
    std::cout << "usage: netSnmpIETFWGTable [-D<tokens>] [-f] [-L] [-M] [-H] [LISTENING ADDRESSES]" << std::endl
		<< "\t-f      Do not fork() from the calling shell." << std::endl
		<< "\t-DTOKEN[,TOKEN,...]" << std::endl
		<< "\t\tTurn on debugging output for the given TOKEN(s)." << std::endl
		<< "\t\tWithout any tokens specified, it defaults to printing" << std::endl
		<< "\t\tall the tokens (which is equivalent to the keyword 'ALL')." << std::endl
		<< "\t\tYou might want to try ALL for extremely verbose output." << std::endl
		<< "\t\tNote: You can't put a space between the -D and the TOKENs." << std::endl
		<< "\t-H\tDisplay a list of configuration file directives" << std::endl
		<< "\t\tunderstood by the agent and then exit." << std::endl
		<< "\t-M\tRun as a normal SNMP Agent instead of an AgentX sub-agent." << std::endl
		<< "\t-x ADDRESS\tconnect to master agent at ADDRESS (default /var/agentx/master)." << std::endl
		<< "\t-L\tDo not open a log file; print all messages to stderr." << std::endl;

    std::exit(0);
}

int
main(int argc, char **argv) {
	int agentx_subagent = 1; /* change this if you want to be a SNMP master agent */
	netSnmpMQTT::netSnmpMQTTTable *snmpMQTTTable;
	/*
	 * Defs for arg-handling code: handles setting of policy-related variables 
	 */
	int ch;
	extern char *optarg;
	int dont_fork = 0, use_syslog = 0;
	char *agentx_socket = NULL;


	while ((ch = getopt(argc, argv, "D:fHLMx:")) != EOF)
		switch (ch) {
			case 'D':
				debug_register_tokens(optarg);
				snmp_set_do_debugging(1);
				break;
			case 'f':
				dont_fork = 1;
				break;
			case 'H':
				netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID,
					NETSNMP_DS_AGENT_NO_ROOT_ACCESS, 1);
                init_agent("netSnmpMQTTTable"); /* register our .conf handlers */
                snmpMQTTTable = new netSnmpMQTT::netSnmpMQTTTable();
                init_snmp("netSnmpMQTTTable");
                std::cerr << "Configuration directives understood:" << std::endl;
				read_config_print_usage("  ");
				exit(0);
			case 'M':
				agentx_subagent = 0;
				break;
			case 'L':
				use_syslog = 0; /* use stderr */
				break;
			case 'x':
				agentx_socket = const_cast< char*>("udp:localhost:705");
				break;
			default:
				fprintf(stderr, "unknown option %c\n", ch);
				usage();
		}

   // use_syslog = 1;
	if (optind < argc) {
		int i;
		/*
		 * There are optional transport addresses on the command line.  
		 */
		DEBUGMSGTL(("snmpd/main", "optind %d, argc %d\n", optind, argc));
		for (i = optind; i < argc; i++) {
			char *c, *astring;
			if ((c = netsnmp_ds_get_string(NETSNMP_DS_APPLICATION_ID,
				NETSNMP_DS_AGENT_PORTS))) {
				astring = (char *) malloc(strlen(c) + 2 + strlen(argv[i]));
				if (astring == NULL) {
					fprintf(stderr, "malloc failure processing argv[%d]\n",
						i);
					exit(1);
				}
				sprintf(astring, "%s,%s", c, argv[i]);
				netsnmp_ds_set_string(NETSNMP_DS_APPLICATION_ID,
					NETSNMP_DS_AGENT_PORTS, astring);
				SNMP_FREE(astring);
			} else {
				netsnmp_ds_set_string(NETSNMP_DS_APPLICATION_ID,
					NETSNMP_DS_AGENT_PORTS, argv[i]);
			}
		}
		DEBUGMSGTL(("snmpd/main", "port spec: %s\n",
			netsnmp_ds_get_string(NETSNMP_DS_APPLICATION_ID,
			NETSNMP_DS_AGENT_PORTS)));
	}

	/*
	 * we're an agentx subagent? 
	 */
	if (agentx_subagent) {
		/*
		 * make us a agentx client. 
		 */
		netsnmp_enable_subagent();
		if (NULL != agentx_socket)
			netsnmp_ds_set_string(NETSNMP_DS_APPLICATION_ID,
			NETSNMP_DS_AGENT_X_SOCKET,
			agentx_socket);
	}

	snmp_disable_log();
	if (use_syslog)
		snmp_enable_calllog();
	else
		snmp_enable_stderrlog();

	/*
	 * daemonize 
	 */
	if (!dont_fork) {
		int rc = netsnmp_daemonize(1, !use_syslog);
		if (rc)
			exit(-1);
	}

	/*
	 * initialize tcp/ip if necessary 
	 */
	SOCK_STARTUP;

	/*
	 * initialize the agent library 
	 */
	init_agent("netSnmpMQTTTable");

	/*
	 * init netSnmpMQTTTable
	 */
    	snmpMQTTTable = new netSnmpMQTT::netSnmpMQTTTable();
	init_snmp("netSnmpMQTTTable");

	/*
	 * If we're going to be a snmp master agent, initial the ports 
	 */
	if (!agentx_subagent)
		init_master_agent(); /* open the port to listen on (defaults to udp:161) */

	/*
	 * In case we recevie a request to stop (kill -TERM or kill -INT) 
	 */
	keep_running = 1;
	signal(SIGTERM, stop_server);
	signal(SIGINT, stop_server);
	
	
//	mosq_snmp_setup(&state);

    TMQTTSNMPHandler::TConfig  mqtt_config;
    mqtt_config.Host = "localhost";
    mqtt_config.Port = 1883;
    mosqpp::lib_init();
    mqtt_config.Id = "SNMP";
    std::shared_ptr<TMQTTSNMPHandler> mqtt_handler(new TMQTTSNMPHandler(mqtt_config, snmpMQTTTable));
    mqtt_handler->Init();


    int rc=0;
    /*
     * you're main loop here...
     */
    if (rc != 0 ) {
        cerr << "couldn't start mosquitto_loop_start ! " << rc << endl;
    } else {

    thread mqttThread([&]() {
	while (keep_running) {
               int rc =  mqtt_handler->loop(50,1);
		if (rc != 0) {
		        mqtt_handler->reconnect();
		}
	}
    });

    thread snmpThread([&]() {
	while (keep_running) {
        		agent_check_and_process(1); /* 0 == don't block */
//			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
    });

    mqttThread.join();
    snmpThread.join();
    mqtt_handler->loop_stop();
    
    }

    mosqpp::lib_cleanup();

	/*
	 * at shutdown time 
	 */
	snmp_shutdown("netSnmpMQTTTable");
	SOCK_CLEANUP;
	exit(0);
}
