p.e.l
=====

Small interprented language geared towards expressing formualaes for particle emission properties. 

	void main()
	{		
		// At abritary locations you can inject a #optimize off or #optimize to control wether 
		// optimizations should be enabled for code following the optimize command.
		#optimize off
	
		// Can assign values to variables, each variables is a float, and stored global and is
		// retained between invocations.
		position.x = 0.4;
		
		// Typical comparision operators
		if( position.x == 0.4 )
		{
			position.x = 0.8;
			position.y = 0.12;
		}		
		
		// Built in trig functions, expected cos/sin/lerp/smoothstep/atan/atan2
		position.x = 0.4 + position.x + cos(position.x) % 0.1f;		
	}
