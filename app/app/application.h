#pragma once
#include <boost/program_options.hpp>

using bf = boost::filesystem;

namespace app {
	class application {

	public:
		application();
		~application();

		void set_program_options(boost::program_options::options_description& command_line_options,
			boost::program_options::options_description& configuration_file_options)const;
		void initialize(const bf::path& data_dir, const boost::program_options::variables_map& options);
				
		void startup();
		void shutdown();
	};
}