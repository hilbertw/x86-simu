#include "application.h"


namespace app {

	application::application()
		: my(new detail::application_impl(this))
	{}

	application::~application()
	{

	}

	void application::set_program_options(boost::program_options::options_description& command_line_options,
		boost::program_options::options_description& configuration_file_options) const
	{
		configuration_file_options.add_options()
			("p2p-endpoint", bpo::value<string>(), "Endpoint for P2P node to listen on")
			("seed-node,s", bpo::value<vector<string>>()->composing(),
				"P2P nodes to connect to on startup (may specify multiple times)")
				("seed-nodes", bpo::value<string>()->composing(),
					"JSON array of P2P nodes to connect to on startup")
					("checkpoint,c", bpo::value<vector<string>>()->composing(),
						"Pairs of [BLOCK_NUM,BLOCK_ID] that should be enforced as checkpoints.")
						("rpc-endpoint", bpo::value<string>()->implicit_value("127.0.0.1:8090"),
							"Endpoint for websocket RPC to listen on")
							("rpc-tls-endpoint", bpo::value<string>()->implicit_value("127.0.0.1:8089"),
								"Endpoint for TLS websocket RPC to listen on")
								("server-pem,p", bpo::value<string>()->implicit_value("server.pem"), "The TLS certificate file for this server")
			("server-pem-password,P", bpo::value<string>()->implicit_value(""), "Password for this certificate")
			("genesis-json", bpo::value<boost::filesystem::path>(), "File to read Genesis State from")
			("dbg-init-key", bpo::value<string>(), "Block signing key to use for init witnesses, overrides genesis file")
			("api-access", bpo::value<boost::filesystem::path>(), "JSON file specifying API permissions")
			("io-threads", bpo::value<uint16_t>()->implicit_value(0), "Number of IO threads, default to 0 for auto-configuration")
			("enable-subscribe-to-all", bpo::value<bool>()->implicit_value(true),
				"Whether allow API clients to subscribe to universal object creation and removal events")
				("enable-standby-votes-tracking", bpo::value<bool>()->implicit_value(true),
					"Whether to enable tracking of votes of standby witnesses and committee members. "
					"Set it to true to provide accurate data to API clients, set to false for slightly better performance.")
					("api-limit-get-account-history-operations", boost::program_options::value<uint64_t>()->default_value(100),
						"For history_api::get_account_history_operations to set its default limit value as 100")
						("api-limit-get-account-history", boost::program_options::value<uint64_t>()->default_value(100),
							"For history_api::get_account_history to set its default limit value as 100")
							("api-limit-get-grouped-limit-orders", boost::program_options::value<uint64_t>()->default_value(101),
								"For orders_api::get_grouped_limit_orders to set its default limit value as 101")
								("api-limit-get-relative-account-history", boost::program_options::value<uint64_t>()->default_value(100),
									"For history_api::get_relative_account_history to set its default limit value as 100")
									("api-limit-get-account-history-by-operations", boost::program_options::value<uint64_t>()->default_value(100),
										"For history_api::get_account_history_by_operations to set its default limit value as 100")
										("api-limit-get-asset-holders", boost::program_options::value<uint64_t>()->default_value(100),
											"For asset_api::get_asset_holders to set its default limit value as 100")
											("api-limit-get-key-references", boost::program_options::value<uint64_t>()->default_value(100),
												"For database_api_impl::get_key_references to set its default limit value as 100")
												("api-limit-get-htlc-by", boost::program_options::value<uint64_t>()->default_value(100),
													"For database_api_impl::get_htlc_by_from and get_htlc_by_to to set its default limit value as 100")
													("api-limit-get-full-accounts", boost::program_options::value<uint64_t>()->default_value(10),
														"For database_api_impl::get_full_accounts to set its account default limit values as 10")
														("api-limit-get-full-accounts-lists", boost::program_options::value<uint64_t>()->default_value(100),
															"For database_api_impl::get_full_accounts to set its lists default limit values as 100")
															("api-limit-get-call-orders", boost::program_options::value<uint64_t>()->default_value(300),
																"For database_api_impl::get_call_orders and get_call_orders_by_account to set its default limit values as 300")
																("api-limit-get-settle-orders", boost::program_options::value<uint64_t>()->default_value(300),
																	"For database_api_impl::get_settle_orders and get_settle_orders_by_account to set its default limit values as 300")
																	("api-limit-get-assets", boost::program_options::value<uint64_t>()->default_value(101),
																		"For database_api_impl::list_assets and get_assets_by_issuer to set its default limit values as 101")
																		("api-limit-get-limit-orders", boost::program_options::value<uint64_t>()->default_value(300),
																			"For database_api_impl::get_limit_orders to set its default limit value as 300")
																			("api-limit-get-order-book", boost::program_options::value<uint64_t>()->default_value(50),
																				"For database_api_impl::get_order_book to set its default limit value as 50")
			;
		command_line_options.add(configuration_file_options);
		command_line_options.add_options()
			("replay-blockchain", "Rebuild object graph by replaying all blocks without validation")
			("revalidate-blockchain", "Rebuild object graph by replaying all blocks with full validation")
			("resync-blockchain", "Delete all blocks and re-sync with network from scratch")
			("force-validate", "Force validation of all transactions during normal operation")
			("genesis-timestamp", bpo::value<uint32_t>(),
				"Replace timestamp from genesis.json with current time plus this many seconds (experts only!)")
			;
		command_line_options.add(_cli_options);
		configuration_file_options.add(_cfg_options);
	}

	void application::initialize(const fc::path& data_dir, const boost::program_options::variables_map& options)
	{
		my->_data_dir = data_dir;
		my->_options = &options;

		if (options.count("io-threads"))
		{
			const uint16_t num_threads = options["io-threads"].as<uint16_t>();
			fc::asio::default_io_service_scope::set_num_threads(num_threads);
		}
	}

	void application::startup()
	{
		try {
			my->startup();
		}
		catch (const fc::exception& e) {
			elog("${e}", ("e", e.to_detail_string()));
			throw;
		}
		catch (...) {
			elog("unexpected exception");
			throw;
		}
	}

}