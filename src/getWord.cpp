#include <string>

// retorna uma palavra (substring) que vai de startIndex até o primeiro separator encontrado
int getWord(std::string str, int startIndex, char separator, std::string *word) { 
  int i = startIndex, endIndex;

  while (i <= str.size()) { 
    if (str[i] == separator || i == str.size())  {  
      endIndex = i;  
      *word = "";
      word->append(str, startIndex, endIndex - startIndex);
      return endIndex + 1;
    } 
    i++;  
  }
  return 0;  
}
