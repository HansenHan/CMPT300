#include "mystring.h"
#include <stdio.h>
#include <stdlib.h>

/*
 *   Implement the following functions: 
 * 
 *   You are NOT allowed to use any standard string functions such as 
 *   strlen, strcpy or strcmp or any other string function.
 */

/*
 *  mystrlen() calculates the length of the string s, 
 *  not including the terminating character '\0'.
 *  Returns: length of s.
 */
int mystrlen (const char *s) 
{
	const char *tmp;
	tmp = s;
	int len  = 0;
	while(*tmp != '\0')
	{
		tmp++;
		len++;
	}
	return len;
	/* Complete the body of the function */
	//return 0;
}

/*
 *  mystrcpy()  copies the string pointed to by src (including the terminating 
 *  character '\0') to the array pointed to by dst.
 *  Returns: a  pointer to the destination string dst.
 */
char  *mystrcpy (char *dst, const char *src)
{
	int len = mystrlen(src);
	for(int i = 0; i <= len; i++){
		dst[i] = src[i];
	}
	/* Complete the body of the function */
	return dst;
}

/*
 * mystrcmp() compares two strings alphabetically
 * Returns: 
 * 	-1 if s1  < s2
 *  	0 if s1 == s2
 *  	1 if s1 > s2
 */
int mystrcmp(const char *s1, const char *s2)
{
	const char *tmp1 = s1;
	const char *tmp2 = s2;
	if(*tmp1 == '\0' && *tmp2 == '\0'){
		return 0;
	}
	int bo = 0;
	for(;*tmp1 != '\0' && *tmp2 != '\0' && bo == 0; ){
		if(*tmp1 > *tmp2){
			bo = 1;
			return 1;
		}
		if(*tmp1 < *tmp2){
			bo = 2;
			return -1;
		}
		if(*tmp1 == *tmp2){
			tmp1++;
    		tmp2++;
		}
	}
	if(*tmp1 == '\0' && *tmp2 == '\0'){
		return 0;
	}
	if(*tmp1 == '\0'){
		return -1;
	}
	if(*tmp2 == '\0'){
		return 1;
	}

    // while(*tmp1 != '\0')
    // {
    // 	if (*tmp2 == '\0') 
    // 		return 1;
    // 	if (*tmp2 > *tmp1)
    // 		return -1;
    // 	if (*tmp1 > *tmp2)
    // 		return 1;
    // 	tmp1++;
    // 	tmp2++;
    // }
    // if(*tmp2 != '\0')
    // 	return -1;

    return 0;



	/* Complete the body of the function */
	//return 0;
}

/*
 * mystrdup() creates a duplicate of the string pointed to by s. 
 * The space for the new string is obtained using malloc.  
 * If the new string can not be created, a null pointer is returned.
 * Returns:  a pointer to a new string (the duplicate) 
 	     or null If the new string could not be created for 
	     any reason such as insufficient memory.
 */
char *mystrdup(const char *s1)
{
	int len = mystrlen(s1);

	if (len == 0) {
		return NULL;
	}

	char *dup = malloc(sizeof(char) * (len));


	for (int i = 0; i < len; i++) {
		dup[i] = s1[i];
	}

	return dup;
	/* Complete the body of the function */

}

