#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <vector>
#include <stack>
#include <string>
#include <stdio.h>
#include <assert.h>
#include <math.h>

enum opcode
{	
	e_ret,
	e_load,
	e_store,
	e_add,
	e_sub,
	e_mul,
	e_div,
	e_mod,

	
	e_jmp,
	e_eq,
	e_lt,
	e_gt,
	e_neq,
	
	e_lfld,
	e_sfld,

	e_sin,
	e_cos,
};

class function;
typedef unsigned int Label;
typedef unsigned int Local;


class function
{
	std::vector<char>  bytecode;	
public:
	std::vector<float> locals;
	std::vector<std::string> localNames;
private:



	void il_add_bytecode_u8( char v )
	{
		bytecode.push_back( v );
	}

	void il_add_bytecode_u32( unsigned int v )
	{
		bytecode.push_back( (v & 0x000000FF) >> 0 );
		bytecode.push_back( (v & 0x0000FF00) >> 8 );
		bytecode.push_back( (v & 0x00FF0000) >> 16 );
		bytecode.push_back( (v & 0xFF000000) >> 24 );
	}



	void il_add_bytecode_flt( float f )
	{
		unsigned int v = *reinterpret_cast<unsigned int*>(&f);
		bytecode.push_back( (v & 0x000000FF) >> 0 );
		bytecode.push_back( (v & 0x0000FF00) >> 8 );
		bytecode.push_back( (v & 0x00FF0000) >> 16 );
		bytecode.push_back( (v & 0xFF000000) >> 24 );
	}

	unsigned int il_decode_u32( char* v )
	{
		return *reinterpret_cast<unsigned int*>( v );
	}



	float il_decode_flt( char* v )
	{
		return *reinterpret_cast<float*>( v );
	}


public:

	function()
	{
		locals.push_back(0.0f);
		localNames.push_back("position.x");
	}


	int  il_size()
	{
		return bytecode.size();
	}

	Label il_get_label()
	{
		return bytecode.size();
	}



	void il_set_label_instr(Label lbl, Label rwr)
	{
		bytecode[lbl + 0] = (rwr & 0x000000FF) >> 0;
		bytecode[lbl + 1] = (rwr & 0x0000FF00) >> 8;
		bytecode[lbl + 2] = (rwr & 0x00FF0000) >> 16;
		bytecode[lbl + 3] = (rwr & 0xFF000000) >> 24;
	}

	Label il_eq(Label lbl)
	{
		il_add_bytecode_u8( e_eq );
		Label l = il_get_label();
		il_add_bytecode_u32( lbl );
		return l;
	}

	Label il_neq(Label lbl)
	{
		il_add_bytecode_u8( e_neq );
		Label l = il_get_label();
		il_add_bytecode_u32( lbl );
		return l;
	}

	Label il_gt(Label lbl)
	{
		il_add_bytecode_u8( e_gt );
		Label l = il_get_label();
		il_add_bytecode_u32( lbl );
		return l;
	}

	Label il_lt(Label lbl)
	{
		il_add_bytecode_u8( e_lt );
		Label l = il_get_label();
		il_add_bytecode_u32( lbl );
		return l;
	}

	Label il_jmp(Label lbl)
	{
		il_add_bytecode_u8( e_jmp );
		Label l = il_get_label();
		il_add_bytecode_u32( lbl );
		return l;
	}

	void il_add()
	{
		il_add_bytecode_u8(e_add);
	}

	void il_sub()
	{
		il_add_bytecode_u8(e_sub);
	}

	void il_mul()
	{
		il_add_bytecode_u8(e_mul);
	}

	void il_div()
	{
		il_add_bytecode_u8(e_div);
	}

	void il_mod()
	{
		il_add_bytecode_u8(e_mod);
	}

	void il_push(float v)
	{
		il_add_bytecode_u8( e_load );
		il_add_bytecode_flt( v );
	}

	void il_sin()
	{
		il_add_bytecode_u8( e_sin );
	}

	void il_cos()
	{
		il_add_bytecode_u8( e_cos );
	}

	void il_pop()
	{
		il_add_bytecode_u8( e_store );
	}

	void il_sfld(Local lbl)
	{
		il_add_bytecode_u8( e_sfld );
		il_add_bytecode_u32( lbl );
	}

	void il_lfld(Local lbl)
	{
		il_add_bytecode_u8( e_lfld );
		il_add_bytecode_u32( lbl );
	}

	void il_ret()
	{
		il_add_bytecode_u8(e_ret);
	}


	void run(std::stack<float>& stack)
	{
		char* v = &bytecode[0];
		while( true ) 
		{
			switch( *(v++) ) 
			{
				case e_ret:
					printf("return function\r\n");
					return;				
				case e_load:
					{
						float i = il_decode_flt(v);
						stack.push(i);
						printf("load stack %f\r\n", i);
						v += 4;
					}
					break;
				case e_store:
					{
						stack.pop();
						printf("store stack\r\n");
					}
					break;
				case e_lfld:
					{
						unsigned int i = il_decode_u32(v);
						printf("load field\r\n");
						stack.push(locals[i]);
						v += 4;
					}
					break;
				case e_sfld:
					{
						unsigned int i = il_decode_u32(v);
						float a = stack.top(); stack.pop();
						printf("store field\r\n");
						locals[i] = a;
						v += 4;
					}
					break;
				case e_add:
					{
						float a = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						stack.push( a + b );
						printf("add %f\r\n", a + b);
					}
					break;
				case e_sub:
					{
						float a = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						stack.push( a - b );
						printf("add %f\r\n", a - b);
					}
					break;
				case e_mul:
					{
						float a = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						stack.push( a * b );
						printf("add %f\r\n", a * b);
					}
					break;
				case e_div:
					{
						float b = stack.top(); stack.pop();
						float a = stack.top(); stack.pop();						
						stack.push( a / b );
						printf("add %f\r\n", a / b);
					}
					break;
				case e_mod:
					{						
						float b = stack.top(); stack.pop();
						float a = stack.top(); stack.pop();
						stack.push( fmodf(a, b) );
						printf("mod %f %f %f\r\n", a, b, fmodf(a, b) );
					}
					break;

				case e_jmp:
					{
						unsigned int i = il_decode_u32(v);
						printf("jmp\r\n");
						v = &bytecode[i];
					}
					break;

				case e_eq:
					{
						unsigned int i = il_decode_u32(v);
						float a = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						printf("eq %f %f == %d\r\n", a, b, a == b);
						if( a == b ) {
							v = &bytecode[i];
						} else {
							v += 4;
						}
					}
					break;
				case e_lt:
					{
						unsigned int i = il_decode_u32(v);
						float a = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						printf("lt %f %f == %d\r\n", a, b, a <= b);
						if( a <= b ) {
							v = &bytecode[i];
						} else {
							v += 4;
						}
					}
					break;
				case e_gt:
					{
						unsigned int i = il_decode_u32(v);
						float a = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						printf("gt %f %f == %d\r\n", a, b, a >= b);
						if( a >= b ) {
							v = &bytecode[i];
						} else {
							v += 4;
						}
					}
					break;
				case e_neq:
					{
						unsigned int i = il_decode_u32(v);
						float a = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						printf("neq %f %f != %d\r\n", a, b, a != b);
						if( a != b ) {
							printf("\t jmp %d\r\n", i);
							v = &bytecode[i];
						} else {
							v += 4;
						}
					}
					break;
				case e_cos:
					{
						float a = stack.top(); stack.pop();
						printf("cos %f\r\n", a);
						stack.push( cos(a) );
					}
					break;
				case e_sin:
					{
						float a = stack.top(); stack.pop();
						printf("sin %f\r\n", a);
						stack.push( sin(a) );
					}
					break;
			}
		}
	}
};

#endif //EXPRESSION_H