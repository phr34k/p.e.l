p.e.l
=====

Small interprented language geared towards expressing formualas for particle emission properties. 

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
		
		// Assignment expressions
		position.x = 0.4 + position.x + cos(position.x) % 0.1f;		
		
		// Built in trig functions
		position.clamp = clamp(0.0, 1.0, -3.0);
		position.lerp = lerp(0.0, 2.0, 1.5);
		position.smth = smoothstep(0.0, 2.0, 1.0);
		position.sqrt = sqrt(100);
		position.abs = abs(-300);	
		position.sign = sign(-300);
		position.rad = radians(120);
		position.deg = degrees(position.rad);
		position.ceil = ceil(2.2f);
		position.floor = floor(2.2f);
		position.roundl = round(2.4899f);
		position.roundh = round(2.5211f);		
	}
