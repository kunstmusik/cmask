//# define PMAX 50


class event {
	char	op;
	long	pcnt;
	double	*val;
	
public:

			event(long);
			~event(void);
	void	setop(char);
	void	setp(long, double);
	void	get(char *s);
	};		

