 * When would you use a sorted binary tree over a linked list?
 	a Binary tree is faster to search and delete

• What would you change if the tree had to support not just integers, but any type of data?
	void *pointer ← has no datatype
	later we address a character variable for example and the void pointer becomes a character pointer 
• Why do we use modules instead of putting everything a single file?
	clear structure
	code reuse
	teamwork
• Is the development cycle (i.e. changing a source file, recompiling and running tests) faster when using modules? Explain your answer.
	If you want to change a source file, you are maybe faster when you are using modules, coz of the clear structure
	There shouldn't be a difference when you recompile or test your code.
• What is a header guard and why is it needed?
	A header guard solves the problem with multiple implementation of header files
	„a implements b.h“ and „c implement a and b.h“ 
• Why are btree and btree_node not defined in the header file? What are the implications?
	private structures -> .c file
	public structures .h file
	If more than one file wants to use the same header file but not the same source code for the structures.
• Explain the const in the parameter list of btree_print, is it required?
	It is impossible to change the data. It is constant. It is some sort of protection but not required
• Explain memory leaks. Why are memory leaks bad? You should know how to use valgrind to check your program for memory leaks.
	When I allocate memory and don't give it back to the system
	It is not good because i can't borrow endless much memory from the heap
	valgrind --tool=memcheck -v ./btree
• What is the reason behind writing everything in English?
	English is one of the main languages in the world. So everybody is able to understand my code. Wouldn‘t be the same in german.
• Why should you use static for non-exported functions?
	Static functions are only visible to functions in the same file
• Why should we comment our source code? Is it always need? What should the comment state? What is self-documenting code?
	It is an explanation for other programmers. So everybody is able to understand/develop my code.
	Self-documenting code leads me to a public website which exposes the process of the code through public documentation
• Why should the module not output debug messages?
	I assume it is confusing
• Why and when should you use assert?
	Assert is for test cases
	It is a diagnostic tool to find errors

• What are the benefits of using make over calling the compiler by hand?
	Faster
	no spelling mistakes
	easier to clean 
• Imagine it was your job to design the interface for the btree module (writing btree.h). What would you have done differently, and why?
	I would combine btree and btree_node because then it is easier to navigate through the tree. 
	Only exception btree_size with O(1) wouldn't be possible anymore.
