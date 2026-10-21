# REAL-TIME OPERATING SYSTEMS
Lab– Porting a Shell
## Reading the code (1/2) p°6

1. **List the functions available to the user of the module (its API). List the hidden ones.**
```
sh_help
shell_init
shell_add
shell_exec
shell_run
```

2. **How is the serial link to write on chosen?**
```
Le serial link choisis est l'UART2 car il s'agit de celui relié à l'ordinateur. 
```

3. **List the static variables and structures. What does the keyword static mean, for a
variable and for a function?**
```c
static int shell_func_list_size = 0;
static shell_func_t shell_func_list[SHELL_FUNC_LIST_MAX_SIZE];

static char print_buffer[BUFFER_SIZE];

static char backspace[] = "\b \b";
static char prompt[] = "> ";
```
```
Le champ "static" pour une variable signifie qu'elle est locale au fichier .c et n'est pas disponible par les autres.
Pour une fonction, cela correspond 
```

4. **How do you add a function callable from the shell?**
```
On écrit la fonction dans le main dans la partie CODE 0.
Sous la forme "int ... (int argc, char ** argv)" puis ajouter avec la fonction shell_add.
Exemple ci-dessous.
```
```c
//Dans USER CODE 0
int add (int argc, char ** argv)
{
	int acc=0;
	for (int i=0; i<argc;i++)
	{
		//Convertir la chaine de caractère en entier
		int num=atoi(argv[i]);
		//Ajouter le nombre à acc
		acc+=num;
	}
	printf("%d\r\n",acc);
	return 0;
}

//Puis dans le USER CODE 2

shell_add('a', add, "Addition");

```

## Reading the code (2/2) p°7

5. In which function are the characters read?
```
Dans uart_read
```

6. What happens when you press Enter?
```
Le programme lis la commande et vérifie si elle existe.
```

7. How does deleting a character work?
```

```

8. To which function is the command string passed?
```

```

9. In that function, how do we end up calling the dummy function?
```

```
