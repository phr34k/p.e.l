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
	e_elt,
	e_egt,
	e_neq,
	
	e_lfld,
	e_sfld,

	e_tan,
	e_sin,
	e_cos,
	e_tanh,
	e_sinh,
	e_cosh,
	e_atan,
	e_asin,
	e_acos,

	e_clamp,
	e_lerp,
	e_smoothstep,
	e_sqrt,
	e_abs,
	e_sign,
	e_radians,
	e_degrees,
	e_ceil,
	e_floor,
	e_round,
	e_rand,
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

	Label il_egt(Label lbl)
	{
		il_add_bytecode_u8( e_egt );
		Label l = il_get_label();
		il_add_bytecode_u32( lbl );
		return l;
	}

	Label il_elt(Label lbl)
	{
		il_add_bytecode_u8( e_elt );
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

	void il_tan()
	{
		il_add_bytecode_u8( e_tan );
	}

	void il_sinh()
	{
		il_add_bytecode_u8( e_sinh );
	}

	void il_cosh()
	{
		il_add_bytecode_u8( e_cosh );
	}

	void il_tanh()
	{
		il_add_bytecode_u8( e_tanh );
	}

	void il_asin()
	{
		il_add_bytecode_u8( e_asin );
	}

	void il_acos()
	{
		il_add_bytecode_u8( e_acos );
	}

	void il_atan()
	{
		il_add_bytecode_u8( e_atan );
	}

	void il_lerp()
	{
		il_add_bytecode_u8( e_lerp );
	}

	void il_clamp()
	{
		il_add_bytecode_u8( e_clamp );
	}

	void il_sqrt()
	{
		il_add_bytecode_u8( e_sqrt );
	}

	void il_abs()
	{
		il_add_bytecode_u8( e_abs );
	}

	void il_sign()
	{
		il_add_bytecode_u8( e_sign );
	}

	void il_radians()
	{
		il_add_bytecode_u8( e_radians );
	}

	void il_degrees()
	{
		il_add_bytecode_u8( e_degrees );
	}

	void il_smoothstep()
	{
		il_add_bytecode_u8( e_smoothstep );
	}

	void il_floor()
	{
		il_add_bytecode_u8( e_floor );
	}

	void il_ceil()
	{
		il_add_bytecode_u8( e_ceil );
	}

	void il_round()
	{
		il_add_bytecode_u8( e_round );
	}

	void il_pop()
	{
		il_add_bytecode_u8( e_store );
	}

	void il_rand()
	{
		il_add_bytecode_u8( e_rand );
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
					#ifndef NDEBUG
					printf("return function\r\n");
					#endif
					return;				
				case e_load:
					{
						float i = il_decode_flt(v);
						stack.push(i);
						#ifndef NDEBUG
						printf("load stack %f\r\n", i);
						#endif
						v += 4;
					}
					break;
				case e_store:
					{
						stack.pop();
						#ifndef NDEBUG
						printf("store stack\r\n");
						#endif 
					}
					break;
				case e_lfld:
					{
						unsigned int i = il_decode_u32(v);
						#ifndef NDEBUG
						printf("load field %f [%d]\r\n", locals[i], i);
						#endif
						stack.push(locals[i]);
						v += 4;
					}
					break;
				case e_sfld:
					{
						unsigned int i = il_decode_u32(v);
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("store field %f [%d]\r\n", a, i);
						#endif
						locals[i] = a;
						v += 4;
					}
					break;
				case e_add:
					{
						float a = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						stack.push( a + b );
						#ifndef NDEBUG
						printf("add %f\r\n", a + b);
						#endif
					}
					break;
				case e_sub:
					{
						float a = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						stack.push( a - b );
						#ifndef NDEBUG
						printf("add %f\r\n", a - b);
						#endif
					}
					break;
				case e_mul:
					{
						float a = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						stack.push( a * b );
						#ifndef NDEBUG
						printf("add %f\r\n", a * b);
						#endif
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
						#ifndef NDEBUG
						printf("mod %f %f %f\r\n", a, b, fmodf(a, b) );
						#endif
					}
					break;

				case e_jmp:
					{
						unsigned int i = il_decode_u32(v);
						#ifndef NDEBUG
						printf("jmp\r\n");
						#endif
						v = &bytecode[i];
					}
					break;

				case e_eq:
					{
						unsigned int i = il_decode_u32(v);
						float a = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("eq %f %f == %d\r\n", a, b, a == b);
						#endif
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
						#ifndef NDEBUG
						printf("lt %f %f == %d\r\n", a, b, a <= b);
						#endif
						if( a < b ) {
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
						#ifndef NDEBUG
						printf("gt %f %f == %d\r\n", a, b, a >= b);
						#endif
						if( a > b ) {
							v = &bytecode[i];
						} else {
							v += 4;
						}
					}
					break;
				case e_elt:
					{
						unsigned int i = il_decode_u32(v);
						float a = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("lt %f %f == %d\r\n", a, b, a <= b);
						#endif
						if( a <= b ) {
							v = &bytecode[i];
						} else {
							v += 4;
						}
					}
					break;
				case e_egt:
					{
						unsigned int i = il_decode_u32(v);
						float a = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("gt %f %f == %d\r\n", a, b, a >= b);
						#endif
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
						#ifndef NDEBUG
						printf("neq %f %f != %d\r\n", a, b, a != b);
						#endif
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
						#ifndef NDEBUG
						printf("cos %f\r\n", a);
						#endif
						stack.push( cos(a) );
					}
					break;
				case e_sin:
					{
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("sin %f\r\n", a);
						#endif
						stack.push( sin(a) );
					}
					break;
				case e_tan:
					{
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("tan %f\r\n", a);
						#endif
						stack.push( tan(a) );
					}
					break;	
				case e_cosh:
					{
						float a = stack.top(); stack.pop();
						printf("cosh %f\r\n", a);
						stack.push( cosh(a) );
					}
					break;
				case e_sinh:
					{
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("sinh %f\r\n", a);
						#endif
						stack.push( sinh(a) );
					}
					break;
				case e_tanh:
					{
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("tanh %f\r\n", a);
						#endif
						stack.push( tanh(a) );
					}
					break;	
				case e_acos:
					{
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("acos %f\r\n", a);
						#endif
						stack.push( acos(a) );
					}
					break;
				case e_asin:
					{
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("asin %f\r\n", a);
						#endif
						stack.push( asin(a) );
					}
					break;
				case e_atan:
					{
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("atan %f\r\n", a);
						#endif
						stack.push( atan(a) );
					}
					break;	
				case e_lerp:
					{
						float c = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("lerp %f %f %f\r\n", a, b, c);
						#endif	
						
						float d = c > 1.0f ? 1.0f : ( c < 0.0f ? 0.0f : c );
						stack.push( a + (b - a) * d );
					}
					break;
				case e_clamp:
					{
						float c = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("clamp %f %f %f\r\n", a, b, c);
						#endif
						float d = c > b ? b : ( c < a ? a : c );
						stack.push( d );
					}
					break;
				case e_smoothstep:
					{
						float c = stack.top(); stack.pop();
						float b = stack.top(); stack.pop();
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("smoothstep %f %f %f\r\n", a, b, c);
						#endif
						
						float r = (c - a) / (b - a);
						float t = r > 1.0f ? 1.0f : ( r < 0.0f ? 0.0f : r );
						stack.push( t * t * (3.0 - 2.0 * t) );
					}
					break;				
				case e_sqrt:
					{
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("sqrt %f\r\n", a);					
						#endif
						stack.push( sqrt(a) );
					}
					break;
				case e_abs:
					{
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("abs %f\r\n", a);					
						#endif
						stack.push( fabs(a) );
					}
					break;
				case e_sign:
					{
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("sign %f\r\n", a);					
						#endif
						stack.push( a < 0 ? -1 : 1 );
					}
					break;
				case e_radians:
					{
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("radians %f\r\n", a);					
						#endif
						stack.push( (3.14159265358979323846f * a) / 180.0f );
					}
					break;
				case e_degrees:
					{
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("degrees %f\r\n", a);					
						#endif
						stack.push( (180 * a) / 3.14159265358979323846f );
					}
					break;

				case e_ceil:
					{
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("ceil %f\r\n", a);					
						#endif
						stack.push( ceil(a) );
					}
					break;
				case e_floor:
					{
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("floor %f\r\n", a);					
						#endif
						stack.push( floor(a) );
					}
					break;
				case e_round:
					{
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("round %f\r\n", a);					
						#endif
						stack.push( a < 0.0 ? ceil(a - 0.5) : floor(a + 0.5) );
					}
					break;			
				case e_rand:
					{
						float b = stack.top(); stack.pop();
						float a = stack.top(); stack.pop();
						#ifndef NDEBUG
						printf("rand %f %f\r\n", a, b);					
						#endif

						double x = (double)rand()/(double)RAND_MAX * (b - a) + a;
						stack.push( x );
					}
					break;	
			}
		}
	}
};

#endif //EXPRESSION_H