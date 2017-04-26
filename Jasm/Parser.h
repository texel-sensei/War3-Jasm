#pragma once
#include "VM.h"
#include "OpVisitor.h"

inline optional<int32_t> try_parse_number(std::string const& s, int base = 10)
{
	try
	{
		return std::stoi(s, nullptr, base);
	}
	catch (...)
	{
		return{};
	}
}

inline optional<float> try_parse_real (std::string const& s)
{
	try
	{
		return std::stof(s);
	}
	catch (...)
	{
		return{};
	}
}

class Parser
{
public:

	explicit Parser(VM const* vm)
		: vm(vm)
	{
	}

	optional<std::vector<opcode>> parse_bytecode(std::istream& data)
	{
		using namespace std;

		OpParser lineparser(this, this->vm);
		string line;
		vector<opcode> ops;
		current_line = 1;
		auto success = true;
		while (getline(data, line))
		{
			auto op = lineparser.parse_line(line);
			if(op)
			{
				ops.push_back(*op);
			} else
			{
				success = false;
			}
			current_line++;
		}
		if (success)
			return ops;
		else
			return {};
	}

private:
	VM const* vm;
	int current_line = 0;

	void LogError(std::string const& message, int step = -1) const
	{
		std::cerr << "Line " << current_line << ": " << message << " at step " << step << std::endl;
	}

	class OpParser : public OpVisitor
	{
	public:
		OpParser(Parser* parser, VM const* vm)
			: parser(parser),
			  vm(vm)
		{
		}

		~OpParser() override{}

		optional<opcode> parse_line(std::string const& line)
		{
			current_line = std::stringstream(line);
			auto oc = opcode{};
			handle_op(oc);
			if (successfull)
				return oc;
			return {};
		}

	protected:
		void start_op(opcode& op) override {
			successfull = true;
			std::string name;
			current_line >> name;
			if(!current_line.good())
			{
				parser->LogError("Failed to parse opcode!");
				successfull = false;
				return;
			}

			auto key = vm->op_names().lookup(name);
			if(!key)
			{
				parser->LogError("Unknown opcode \""+name + "\"");
				successfull = false;
				return;
			}

			op.optype = *key;
		}

		void handle_unknown(opcode& op, uint32_t& i) override {
			if (!successfull) return;
			current_line >> std::hex >> i;
			if(current_line.fail())
			{
				parser->LogError("Failed to parse an parameter with unknown function!", current_parameter_id());
				successfull = false;
			}
		}

		void handle_ignored(opcode& op, uint32_t& i) override {
			if (!successfull) return;
			current_line >> std::ws;
			if (current_line.peek() == '-')
			{
				current_line.ignore();
			}
		}
		void handle_register(opcode& op, Register& r) override {
			if (!successfull) return;
			
			char c;			
			current_line >> c;
			if(c != 'R')
			{
				parser->LogError("Failed to parse register!", current_parameter_id());
				successfull = false;
				return;
			}

			int i;
			current_line >> i;
			if(i < 0 || i > 255)
			{
				parser->LogError(
					"Register number out of bounds, must be >= 0 and <= 255, was " + std::to_string(i)
					, current_parameter_id()
				);
				successfull = false;
				return;
			}

			r = Register(i);
		}

		void handle_variable(opcode& op, VariableId& v) override
		{
			if (!successfull) return;
			using namespace std;

			string name;
			auto id = parse_prefixed_number("V_", "variable", &name);
			auto is_name = !name.empty();

			if(is_name)
			{
				parser->LogError(
					"Name lookup for variables not yet implemented. \"" + name + "\""
					, current_parameter_id()
				);
				successfull = false;
				return;
			}

			if(id)
			{
				v = VariableId(*id);
			}
		}

		void handle_type(opcode& op, Type& t) override{
			std::string name;
			current_line >> name;
			auto parsed = parse_type(name);
			if(parsed)
			{
				t = *parsed;
				return;
			}

			parser->LogError(
				"Failed to parse type name! \"" + name + "\""
				, current_parameter_id()
			);
			successfull = false;
		}
		void handle_function(opcode& op, FunctionId& fun) override{
			if (!successfull) return;
			using namespace std;

			std::string name;
			auto id = parse_prefixed_number("F_", "function", &name);
			auto is_name = !name.empty();

			if(is_name && op.optype == OP_NATIVE)
			{
				auto native_id = vm->natives().lookup(name);
				if(!native_id)
				{
					parser->LogError(
						"Unknown native \"" + name + "\""
						, current_parameter_id()
					);
					successfull = false;
				}
				fun = native_id.value_or(FunctionId(0));
				return;
			} 
			if(is_name && op.optype == OP_JASSCALL)
			{
				parser->LogError(
					"Name lookup for jass calls is not yet supported! \"" + name + "\""
					, current_parameter_id()
				);
				successfull = false;
				return;
			}

			if (is_name && op.optype == OP_FUNCTION)
			{
				parser->LogError(
					"Automatic ID generation is not yet supported! \"" + name + "\""
					, current_parameter_id()
				);
				successfull = false;
				return;
			}

			if (id)
			{
				fun = FunctionId(*id);
			}
		}

		void handle_label(opcode& op, LabelId& lab) override{
			if (!successfull) return;
			using namespace std;

			string name;
			auto id = parse_prefixed_number("L_", "label", &name);
			auto is_name = !name.empty();

			if (is_name)
			{
				parser->LogError(
					"Name lookup for labels not yet implemented. \"" + name + "\""
					, current_parameter_id()
				);
				successfull = false;
				return;
			}

			if (id)
			{
				lab = LabelId(*id);
			}
		}
		void handle_integer(opcode& op, uint32_t& i) override
		{
			if (!successfull) return;
			using namespace std;

			string name;
			current_line >> name;
			
			auto fail = [&] {
				parser->LogError(
					"Failed to parse data! \"" + name + "\""
					, current_parameter_id()
				);
				successfull = false;
			};

			auto t = get_jass_type(op);
			if(t)
			{
				switch (static_cast<TYPES>(t->value()))
				{
				case J_NOTHING: break;
				case J_NULL: break;
				case J_CODE: break;
				case J_INTEGER:
					{
						auto parsed = try_parse_number(name, 0);
						if (!parsed) { fail(); return; }
						i = *parsed;
						return;
					}
				case J_REAL: {
					auto parsed = try_parse_real(name);
					if (!parsed) { fail(); return; }
					i = *reinterpret_cast<int*>(&*parsed);
					return;
				}
				case J_STRING: break;
				case J_HANDLE: break;
				case J_BOOLEAN: break;
				case J_INTARRAY: break;
				case J_REALARRAY: break;
				case J_STRARRAY: break;
				case J_HARRAY: break;
				case J_BOOLARRAY: break;
				default:;
				}
			}

			// parse as binary, i.e. hexadecimal int
			auto parsed = try_parse_number(name, 16);
			if (parsed) { i = *parsed; return; }
			fail();
			return;
		}
		void pre_parameter(opcode& op) override{
			current_line >> std::ws;
			current_line.clear();
		}

	private:
		Parser* parser;
		VM const* vm;
		std::stringstream current_line;
		bool successfull = true;

		optional<int> parse_prefixed_number(
			std::string const& expected_prefix,
			std::string const& error_msg_name,
			std::string* parsed_name
		){
			using namespace std;

			auto is_name = false;
			if (current_line.peek() == '&')
			{
				current_line.ignore();
				is_name = true;
			}

			string name;
			current_line >> name;
			if (current_line.fail())
			{
				parser->LogError(
					"Failed to parse "+error_msg_name+" parameter!"
					, current_parameter_id()
				);
				successfull = false;
				return {};
			}

			if(is_name)
			{
				*parsed_name = name;
				return {};
			}

			auto len = expected_prefix.length();

			if(len > name.length())
			{
				parser->LogError(
					"Failed parsing " + error_msg_name + " id!"
					" Possible values are either numbers, prefixed numbers '"
					+ expected_prefix + "XXX' or names &SomeIdentifier. Got instead: \"" + name + "\""
					, current_parameter_id()
				);
				return {};
			}

			auto prefix = name.substr(0, len);
			if (prefix != expected_prefix)
			{

				// try to extract an number
				auto id = try_parse_number(name, 16);
				if (id)
				{
					return id;
				}

				parser->LogError(
					error_msg_name + " Id should start with '"+expected_prefix+"', offending value: \"" + name + "\""
					, current_parameter_id()
				);
				successfull = false;
				return {};
			}

			auto suffix = name.substr(len, name.length() - len);
			auto id = try_parse_number(suffix, 16);
			if (id)
			{
				return id;
			}

			parser->LogError(
				"Failed to parse " + error_msg_name +" id! \"" + name + "\""
				, current_parameter_id()
			);
			successfull = false;
			return {};
		}
	};
};