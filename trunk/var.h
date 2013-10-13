struct env {
	char name[100];
	char value[100];

	struct env *next;
};

typedef struct env *EnvP;
EnvP head,alias;
