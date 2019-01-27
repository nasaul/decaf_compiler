%{
#include "errors.h"
#include <string>
#include <iostream>
using namespace std;
int state = 1;
extern "C" int yylex();
%}

%%
  /* Inicia comentario una linea*/
.*"//".*  {
  string text, text1;
  int id_comment;
  text = strdup(yytext);
  id_comment = text.find("//");
  text1 = text.substr(0, id_comment);
  cout << text1;
  break;
}
  /* Inicia comentario multilinea */
.*"/*".* {
  /* Estado 0 para indicar que estamos dentro de comentarios */
  string text, text1;
  int id_comment;
  text = strdup(yytext);
  id_comment = text.find("/*");
  text1 = text.substr(0, id_comment);
  cout << text1;
  state = 0;
  break;
}
  /* Termina comentario multilinea */
.*"*/" {
  /* Estado natural */
  state = 1;
  break;
}
  /* Todo lo demas */
.* {
  /* Ignora texto que está dentro de comentarios multilinea */
  if(state == 0){
      break;
  } else {
    /* Imprime el texto */
    printf("%s", yytext);
    break;
  }
}
<<EOF>> {
  /* Si termina y no han cerrado los comentarios, entonces error */
  if(state == 0){
    ReportError::UntermComment();
    return 1;
  } else{
    return 1;
  }
}
%%
