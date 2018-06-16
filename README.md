# Yg(g)rdrasil

Ygrdrasil General Data tRee Storage and Interchange Language


## Supported data types

+-----------+-----------------------------+
| Type      | Description                 |
+-----------+-----------------------------+
| i8        | 8-bit signed integer        |
| i10       | 16-bit signed integer       |
| i20       | 32-bit signed integer       |
| i40       | 64-bit signed integer       |
| u8        | 8-bit unsigned integer      | 
| u10       | 16-bit unsigned integer     |
| u20       | 32-bit unsigned integer     |
| u40       | 64-bit unsigned integer     |
| f20       | 32-bit float                |
| f40       | 64-bit float                |
| str       | UTF-8 encoded text          |
| obj       | A collection of keys unique |
|           | within the object and       |
|           | corresponding values, where |
|           | a value is an type found    |
|           | within this list.           |
+-----------+-----------------------------+


    compound:obj
		{
		a\ string:str{Lorem\ Ipsum}  # White space are ignored unless preceded by \
		# Array elements may be delimited by white-space or `;`. Empty elements are ignored
		an\ array\ of\ integers:i32[1 2 3 4;; 5 6 7 8]
		}
