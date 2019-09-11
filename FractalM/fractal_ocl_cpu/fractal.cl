	__kernel void calc(__global const int *cr, __global const int *ci, __global int *C) 
	{
		int mn = 256;
		double zr = 0.0, zi = 0.0;
		int i = get_global_id(0);
		while (i < mn && zr * zr + zi * zi < 4.0)
		{
			double temp = zr * zr - zi * zi + cr[0];
			zi = 2.0 * zr * zi + ci[0];
			zr = temp;
			i++;
		}
		C[j] = i;
	}