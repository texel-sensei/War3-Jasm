// Jasm.cpp : Defines the entry point for the console application.
//

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "opcode.h"
#include "op_desc.h"
#include <cassert>
#include "op_format.h"
#include "VM.h"
#include "Parser.h"
#include "JasmPrinter.h"
#include "cxxopts.hpp"
using namespace std;

vector<opcode> read_bytecode_from_preloader_file(std::string const& filename)
{
	ifstream in(filename);
	string line;
	vector<opcode> ops;
	while (getline(in, line))
	{
		int32_t partA, partB;
		
		auto n = sscanf_s(line.c_str(), "\tcall Preload(%*s %d %d %*s", &partA, &partB);

		if(n == 2) {
			auto op = build_opcode(partA, partB);
			ops.push_back(op);
		}
	}
	return ops;
}

void dump_jass_code(string const& filename, VM const& vm)
{
	ofstream jasscode(filename);
	jasscode << "private function init_arrays takes nothing returns nothing\n";
	for (auto sig : signatures)
	{
		if (!sig.code) continue;

		jasscode << "\tset Jasm_Op_Names  [0x" << hex << static_cast<int>(sig.code) 
		<< "] = \"" << get_opcode_name(sig.code, vm)
		<< "\"\n";
		
		for (int i = 0; i < 3; ++i) {
			jasscode << "\tset Jasm_Op_types_" << i << "[0x"
				<< hex << static_cast<int>(sig.code)
				<< "] = " << static_cast<int>(sig.small_pars[2 - i]) << "\n";
		}
		jasscode << "\tset Jasm_Op_types_L[0x"
			<< hex << static_cast<int>(sig.code)
			<< "] = " << static_cast<int>(sig.large_par) << "\n";
	}
	jasscode << "endfunction\n";
	jasscode << "private function init_natives takes nothing returns nothing\n";
	for(auto& kvp : vm.natives())
	{
		jasscode << "\tcall SaveStr(Jasm_KnownSymbols, 0x"
			<< hex << static_cast<int>(OP_NATIVE)
			<< ", 0x" << hex << kvp.first.value()
			<< ", \"" << kvp.second << "\")\n";
	}
	jasscode << "endfunction\n";
}

inline bool ends_with(std::string const & value, std::string const & ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

enum class input_type
{
	unknown,
	jasm,
	decimal,
	binary,
	preloader
};

enum class output_type
{
	unknown,
	jasm,
	binary,
	jass
};

input_type guess_type(string const& filename)
{
	if (ends_with(filename, ".jasm")) return input_type::jasm;
	if (ends_with(filename, ".jobj")) return input_type::binary;
	if (ends_with(filename, ".j")) return input_type::preloader;
	return input_type::unknown;
}

vector<opcode> load_code(input_type it, VM const& vm, const string& filename)
{
	if(it == input_type::binary)
	{
		ifstream in(filename, ios_base::in | ios_base::binary | ios_base::ate);
		auto size = in.tellg();
		if(size % sizeof(opcode) != 0)
		{
			cerr << "File " << filename << " has invalid size!" << endl;
			exit(1);
		}
		in.seekg(0);
		vector<opcode> ops(size / sizeof(opcode));
		in.read(reinterpret_cast<char*>(ops.data()), size);
		return ops;
	}

	if(it == input_type::preloader)
	{
		return read_bytecode_from_preloader_file(filename);
	}

	if(it == input_type::jasm)
	{
		auto p = Parser(&vm);
		ifstream in(filename);
		auto parsed = p.parse_bytecode(in);
		if(!parsed)
		{
			cerr << "Failed parsing file " << filename << endl;
			exit(1);
		}
		return *parsed;
	}
	if(it == input_type::decimal)
	{
		return vm.load_bytecode(filename);
	}
	cerr << "Invalid input_type in load_code" << endl;
	exit(1);
}

int main(int argc, char* argv[])
{
	// setup program options
	vector<string> files;
	cxxopts::Options options(argv[0], "(Dis)assembles jass bytecode to jasm");
	options.positional_help("<input files...>");

	string jass_arrayname;

	options.add_options()
		("h,help","Print help")
		("input", "List of input files", cxxopts::value<vector<string>>())
		;

	options.add_options("output")
		("a,output-jass-array", "Print bytecode in a form that can be pasted into WE", cxxopts::value<string>(jass_arrayname)->implicit_value("jasm_bytecode"))
		("b,output-jobj", "Output binary data")
		("j,output-jasm", "Output jasm code")
		("no-native-ids", "Does not print the function id in a native call")
		("o,output", "Specify an output file", cxxopts::value<string>()->default_value("a.out"))
		;

	options.add_options("input")
		("input-jasm", "Input files are jasm")
		("input-jobj", "Input files are binary bytecode")
		("input-preload", "Input files are dumps by jass preload() exploit")
		("input-decimal", "Input files containt bytecode as two decimal integers per line")
		;

	options.parse_positional("input");
	try{
		options.parse(argc, argv);
	} catch(exception const& e)
	{
		cerr << "Got exception: " << e.what() << endl;
		return 1;
	}
	

	if(options.count("help"))
	{
		cout << options.help({"", "output", "input"}) << endl;
		return 0;
	}
	
	if(options.count("input") == 0)
	{
		cerr << "No input files!" << endl;
		return 1;
	}

	files = options["input"].as<vector<string>>();

	auto it = input_type::unknown;
	auto set_input_type = false;

	if(options.count("input-jasm"))
	{
		set_input_type = true;
		it = input_type::jasm;
	}

	if(options.count("input-jobj"))
	{
		if(set_input_type)
		{
			cerr << "Can only specify one input flag!" << endl;
			cerr << options.help({ "input" }) << endl;
			return 1;
		}
		set_input_type = true;
		it = input_type::binary;
	}

	if (options.count("input-preload"))
	{
		if (set_input_type)
		{
			cerr << "Can only specify one input flag!" << endl;
			cerr << options.help({ "input" }) << endl;
			return 1;
		}
		set_input_type = true;
		it = input_type::preloader;
	}

	if (options.count("input-decimal"))
	{
		if (set_input_type)
		{
			cerr << "Can only specify one input flag!" << endl;
			cerr << options.help({ "input" }) << endl;
			return 1;
		}
		set_input_type = true;
		it = input_type::decimal;
	}

	// init vm
	VM vm;
	vm.load_natives("natives.txt");
	vm.load_op_names("op_names.txt");

	using bytecode = vector<opcode>;
	vector<bytecode> collected_code;

	auto single_input_type = true;
	for (auto&& f : files)
	{
			
		auto input = it;
		if(!set_input_type)
		{
			auto guessed = guess_type(f);
			if(input != input_type::unknown && guessed != input)
			{
				single_input_type = false;
			}
			input = guessed;
			it = guessed;
		}
		if(input == input_type::unknown)
		{
			cerr << "Failed to guess input type for file " << f << endl;
			cerr << "Please specify!" << endl;
			cerr << '\n' <<options.help({ "input" }) << endl;
			exit(1);
		}
		collected_code.push_back(load_code(it,vm, f));
	}	

	auto ot = output_type::unknown;
	auto ot_set = false;

	if(options.count("output-jobf"))
	{
		ot = output_type::binary;
		ot_set = true;
	}

	if (options.count("output-jasm"))
	{
		if(ot_set)
		{
			cerr << "Only specify one output type!" << endl;
			cerr << options.help({ "output" }) << endl;
			return 1;
		}
		ot = output_type::jasm;
		ot_set = true;
	}

	if (options.count("output-jass-array"))
	{
		if (ot_set)
		{
			cerr << "Only specify one output type!" << endl;
			cerr << options.help({ "output" }) << endl;
			return 1;
		}
		ot = output_type::jass;
		ot_set = true;
	}

	if(!ot_set && single_input_type)
	{
		switch(it)
		{
		case input_type::jasm: 
			ot = output_type::binary; 
			break;
		case input_type::binary: 
		case input_type::preloader: 
		case input_type::decimal:
			ot = output_type::jasm;  
			break;
		}
	}

	if(ot == output_type::unknown)
	{
		cerr << "Failed to figure out output type. Please specify!" << endl;
		cerr << options.help({ "output" }) << endl;
		return 1;
	}
	
	assert(ot != output_type::unknown);

	if (ot == output_type::jass)
	{
		if (jass_arrayname.empty())
		{
			cerr << "Missing arrayname parameter to -a" << endl;
			exit(1);
		}
	}

	int mode = ios_base::out;
	if (ot == output_type::binary)
		mode = mode | ios_base::binary;
	ofstream out(options["output"].as<string>(), mode);

	for(auto&& code : collected_code)
	{
		if (ot == output_type::jass)
		{
			jasm_jass_printer printer(vm, out, jass_arrayname);
			printer.print_jasm(code);
		}
		else if(ot == output_type::jasm)
		{
			jasm_printer printer(vm, out);
			printer.show_native_ids = !options.count("no-native-ids");
			printer.print_jasm(code);
		}
		else if(ot == output_type::binary)
		{
			out.write(reinterpret_cast<const char*>(code.data()), code.size() * sizeof(opcode));
		}
	}


	//dump_jass_code("arrays.j", vm);

	system("pause");
    return 0;
}

