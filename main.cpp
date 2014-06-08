#include "expression.h"
#include "Stdio.h"
#include "coco/SymbolTable.h"
#include "coco/Parser.h"
#include "coco/Scanner.h"
#include <sys/timeb.h>
#include <wchar.h>
#include <map>

using namespace Taste;


class visitor
{
public:
	enum pass
	{
		Normal,
		Post,
	};

	std::stack<Label> trueStack;
	std::stack<Label> falseStack;
	std::map<Exp*, Label> jmp;
	std::map<Exp*, Label> labels_0;
	std::map<Exp*, Label> labels_1;
	std::map<Exp*, Label> labels_2;
	std::stack<bool>	operatorStack;

	void visit(Exp* expression, function& v, pass x = Normal)
	{
		if( expression == 0 )
		{
			return;
		}

		dynamic_cast<BlockExpr*>(expression) ? visit(dynamic_cast<BlockExpr*>(expression), v, x) : 
		dynamic_cast<LiteralExpr*>(expression) ? visit(dynamic_cast<LiteralExpr*>(expression), v, x) : 
		dynamic_cast<AssignExpr*>(expression) ? visit(dynamic_cast<AssignExpr*>(expression), v, x) : 
		dynamic_cast<ArthimeticExp*>(expression) ? visit(dynamic_cast<ArthimeticExp*>(expression), v, x) : 
		dynamic_cast<IdentExpr*>(expression) ? visit(dynamic_cast<IdentExpr*>(expression), v, x) : 
		dynamic_cast<CallExpr*>(expression) ? visit(dynamic_cast<CallExpr*>(expression), v, x) : 
		dynamic_cast<Condition*>(expression) ? visit(dynamic_cast<Condition*>(expression), v, x) : 
		dynamic_cast<ComparisonExp*>(expression) ? visit(dynamic_cast<ComparisonExp*>(expression), v, x) : 		
		dynamic_cast<OrExpr*>(expression) ? visit(dynamic_cast<OrExpr*>(expression), v, x) : 		
		dynamic_cast<AndExpr*>(expression) ? visit(dynamic_cast<AndExpr*>(expression), v, x) : 		
		visit( static_cast<Exp*>(0), v, x );
	}

private:
	void visit(BlockExpr* expression, function& v, pass x)
	{		
		for( int i = 0; i < expression->statements.size(); ++i ) {
			visit(expression->statements[i], v);

			//Pop the value from the stack.
			if( !(dynamic_cast<AssignExpr*>(expression->statements[i]) || dynamic_cast<Condition*>(expression->statements[i])) ) 
			{
				v.il_pop();
			}
		}
	}

	void visit(LiteralExpr* expression, function& v, pass x)
	{		
		v.il_push( expression->literal );

		/*
		//Check if the stack needs to be popped.
		AssignExpression* assign = dynamic_cast<AssignExpression*>(expressions[i]);
		Conditional* cond = dynamic_cast<Conditional*>(expressions[i]);
		if( assign == 0 && cond == 0 ) {
			v.il_pop();
		}
		*/
	}

	void visit(AssignExpr* expression, function& v, pass x)
	{		
		visit(expression->exp, v);
		std::string variable( expression->value.begin(), expression->value.end() );
		bool fieldWasFound = false;
		for( int i = 0; i < v.localNames.size(); ++i ) {
			std::string& s = v.localNames[i];
			std::string& d = variable;
			if( s.compare(d) == 0)
			{
				v.il_sfld( i );
				fieldWasFound = true;
			}
		}

		if( fieldWasFound == false )
		{
			int s = v.localNames.size();
			v.localNames.push_back(variable);
			v.locals.push_back(0.0f);
			v.il_sfld( s );
		}

		
	}


	void visit(ArthimeticExp* expression, function& v, pass x)
	{		
		visit(expression->a, v);
		visit(expression->b, v);
		
		switch( expression->op )
		{
			case 1:
				v.il_add();
				break;
			case 2:
				v.il_sub();
				break;
			case 3:
				v.il_mul();
				break;
			case 4:
				v.il_div();
				break;
			case 5:
				v.il_mod();
				break;
			default:
				assert(false);
				break;
		};
	}

	void visit(IdentExpr* expression, function& v, pass x)
	{		
		std::string variable( expression->value.begin(), expression->value.end() );
		bool fieldWasFound = false;
		for( int i = 0; i < v.localNames.size(); ++i ) {
			std::string& s = v.localNames[i];
			std::string& d = variable;
			if( s.compare(d) == 0)
			{
				v.il_lfld( i );
				fieldWasFound = true;
			}
		}

		if( fieldWasFound == false )
		{
			v.il_push(0.0f);
		}
	}





	void visit(CallExpr* expression, function& v, pass x)
	{		
		if( expression->functionName == L"sin" )
		{
			visit(expression->arguments[0], v);
			v.il_sin();
		}
		else if( expression->functionName == L"cos" )
		{
			visit(expression->arguments[0], v);
			v.il_cos();
		}
		else if( expression->functionName == L"tan" )
		{
			visit(expression->arguments[0], v);
			v.il_tan();
		}
		else if( expression->functionName == L"sinh" )
		{
			visit(expression->arguments[0], v);
			v.il_sinh();
		}
		else if( expression->functionName == L"cosh" )
		{
			visit(expression->arguments[0], v);
			v.il_cosh();
		}
		else if( expression->functionName == L"tanh" )
		{
			visit(expression->arguments[0], v);
			v.il_tanh();
		}
		else if( expression->functionName == L"asin" )
		{
			visit(expression->arguments[0], v);
			v.il_asin();
		}
		else if( expression->functionName == L"acos" )
		{
			visit(expression->arguments[0], v);
			v.il_acos();
		}
		else if( expression->functionName == L"atan" )
		{
			visit(expression->arguments[0], v);
			v.il_atan();
		}
		else if( expression->functionName == L"lerp" )
		{
			visit(expression->arguments[0], v);
			visit(expression->arguments[1], v);
			visit(expression->arguments[2], v);
			v.il_lerp();
		}
		else if( expression->functionName == L"smoothstep" )
		{
			visit(expression->arguments[0], v);
			visit(expression->arguments[1], v);
			visit(expression->arguments[2], v);
			v.il_smoothstep();
		}
		else if( expression->functionName == L"clamp" )
		{
			visit(expression->arguments[0], v);
			visit(expression->arguments[1], v);
			visit(expression->arguments[2], v);
			v.il_clamp();
		}
		else if( expression->functionName == L"sqrt" )
		{
			visit(expression->arguments[0], v);
			v.il_sqrt();
		}
		else if( expression->functionName == L"abs" )
		{
			visit(expression->arguments[0], v);
			v.il_abs();
		}
		else if( expression->functionName == L"sign" )
		{
			visit(expression->arguments[0], v);
			v.il_sign();
		}
		else if( expression->functionName == L"radians" )
		{
			visit(expression->arguments[0], v);
			v.il_radians();
		}
		else if( expression->functionName == L"degrees" )
		{
			visit(expression->arguments[0], v);
			v.il_degrees();
		}

	}


	void visit(Condition* expression, function& v, pass x)
	{		
		operatorStack.push(true);
		visit(expression->booleanExpression, v);
		operatorStack.pop();
		assert(operatorStack.size() == 0 );

		//Get the start of the method body
		Label _start = v.il_get_label();
		//Generate the body of the if-clause
		visit(expression->blockExpression, v);
		//Generate a jump instruction to jump back to the main body
		Label _jmp = v.il_jmp( 0 );

		//Obtain the end pointers (TODO: stack these? )
		Label _end = v.il_get_label();	
		v.il_set_label_instr(_jmp, _end);	

		operatorStack.push(true);
		trueStack.push(_start);
		falseStack.push(_end);
		visit(expression->booleanExpression, v, Post);
		trueStack.pop();
		falseStack.pop();
		operatorStack.pop();
		assert( trueStack.size() == 0 );
		assert( falseStack.size() == 0 );
		assert( operatorStack.size() == 0 );
	}

	void visit(ComparisonExp* expression, function& v, pass x)
	{		
		if( x == Normal )
		{
			/*
			case 1: printft(indent, "==\r\n"); break;
			case 2: printft(indent, "!=\r\n"); break;									
			case 3: printft(indent, "<=\r\n"); break;
			case 4: printft(indent, ">=\r\n"); break;
			case 5: printft(indent, "<\r\n"); break;
			case 6: printft(indent, ">\r\n"); break;
			*/

			if( expression->op == 1 )
			{
				if( operatorStack.top() == true )
				{
					//First expression
					visit(expression->a, v, x);
					visit(expression->b, v, x);
					jmp[expression] = v.il_neq( 0 );
				}
				else
				{
					//First expression
					visit(expression->a, v, x);
					visit(expression->b, v, x);
					jmp[expression] = v.il_eq( 0 );
				}
			}
			else if( expression->op == 2 )
			{
				if( operatorStack.top() == true )
				{
					//First expression
					visit(expression->a, v, x);
					visit(expression->b, v, x);
					jmp[expression] = v.il_eq( 0 );
				}
				else
				{
					//First expression
					visit(expression->a, v, x);
					visit(expression->b, v, x);
					jmp[expression] = v.il_neq( 0 );
				}
			}
		}
		else
		{
			if( operatorStack.top() == true )
			{
				v.il_set_label_instr(jmp[expression], falseStack.top());
			}
			else
			{
				v.il_set_label_instr(jmp[expression], trueStack.top());
			}
		}
	}


	void visit(OrExpr* expression, function& v, pass x)
	{		
		if( x == Normal )
		{
			operatorStack.push(false);
			labels_0[expression] = v.il_get_label();
			visit(expression->a, v, x);
			labels_1[expression] = v.il_get_label();
			visit(expression->b, v, x);
			labels_2[expression] = v.il_get_label();		
			jmp[expression] = v.il_jmp(0);
			operatorStack.pop();
		}
		else
		{
			//If expressions is true, return the flow to the parent true case, otherwise evaluate the second pair.
			operatorStack.push(false);
			trueStack.push( trueStack.top() ); falseStack.push( labels_1[expression] );
			visit(expression->a, v, x);
			trueStack.pop(); falseStack.pop();
			operatorStack.pop();

			//If expressions is true, return the flow to the parent true case, otherwise return flow to the parents false case.
			operatorStack.push(false);
			trueStack.push( trueStack.top() ); falseStack.push( falseStack.top() );
			visit(expression->b, v, x);
			trueStack.pop(); falseStack.pop();
			operatorStack.pop();

			//All the operations failed go back to the parents true case
			v.il_set_label_instr(jmp[expression], falseStack.top() );
		}
	}


	void visit(AndExpr* expression, function& v, pass x)
	{		
		if( x == Normal )
		{
			operatorStack.push(true);
			labels_0[expression] = v.il_get_label();
			visit(expression->a, v, x);
			labels_1[expression] = v.il_get_label();
			visit(expression->b, v, x);
			labels_2[expression] = v.il_get_label();		
			jmp[expression] = v.il_jmp(0);
			operatorStack.pop();
		}
		else
		{
			//If expressions is true, return the flow to the parent true case, otherwise evaluate the second pair.
			operatorStack.push(true);
			trueStack.push( labels_1[expression] ); falseStack.push( falseStack.top() );
			visit(expression->a, v, x);
			trueStack.pop(); falseStack.pop();
			operatorStack.pop();

			//If expressions is true, return the flow to the parent true case, otherwise return flow to the parents false case.
			operatorStack.push(true);
			trueStack.push( trueStack.top() ); falseStack.push( falseStack.top() );
			visit(expression->b, v, x);
			trueStack.pop(); falseStack.pop();
			operatorStack.pop();

			//All the operations failed go back to the parents true case
			v.il_set_label_instr(jmp[expression], trueStack.top() );
		}
	}	
};

int main (int argc, char *argv[]) {

	if (argc == 2 ) 
	{
		wchar_t *fileName = coco_string_create(argv[1]);
		Taste::Scanner *scanner = new Taste::Scanner(fileName);
		Taste::Parser *parser = new Taste::Parser(scanner);
		parser->Parse();
			
		if (parser->errors->count == 0) 
		{
			parser->results->eval(0);
			visitor gen; function z;
			gen.visit(parser->results, z);		

			printf("\r\n");
			printf("\r\n");
			z.il_ret();
			z.run( std::stack<float>() );
			printf("\r\n");
			printf("\r\n");
			
			printf("bytecode size: %d\r\n", z.il_size());
			printf("locals size: %d\r\n", z.locals.size() * sizeof(float));
			printf("combined: %d\r\n", (z.locals.size() * sizeof(float)) + z.il_size());
			for( int i = 0; i < z.localNames.size(); ++i )
				printf("%s = %f\r\n", z.localNames[i].c_str(), z.locals[i] );
			printf("\r\n");
			printf("\r\n");
		}

		coco_string_delete(fileName);
		//delete parser->gen;
		//delete parser->tab;
		delete parser;
		delete scanner;		
	} else {
		printf("-- No source file specified\n");
	}

	#if 0
	function x;

	Label restart = x.il_get_label();
	x.il_push(3.14);
	x.il_push(1.14);
	x.il_add();
	

	x.il_push(4.28);
	x.il_neq(restart);
	x.il_ret();

	std::stack<float> stack;
	x.run(stack);
	#endif
	
	getchar();
	return 0;

}