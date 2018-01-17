/* String functions
 *
 * (c) Copyright 2005-2011, ITB CompuPhase
 * This file is provided as is (no warranties).
 */
#pragma library String

native strlen(const string[]);
native sprintf(dest[], maxlength=sizeof dest, const format[], {Float,Fixed,_}:...);
native strcpy(dest[], const source[], maxlength=sizeof dest);
native strcat(dest[], const source[], maxlength=sizeof dest);
native strcmp(const string1[], const string2[], length=cellmax);
native strchr(const string[], char);
native strrchr(const string[], char);
native strspn(const string1[], const string2[]);
native strcspn(const string1[], const string2[]);
native strpbrk(const string1[], const string2[]);
native strstr(const string1[], const string2[]);
native strtol(const string[], base);
native Float: atof(const string[]);

stock atoi(const string[])
  return strtol(string, 10);

