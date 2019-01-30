%{
#include "errors.h"
#include <string>
#include <iostream>
#include <map>
using namespace std;
std::map<std::string, std::string> macros;
int state = 1;
int number_line = 1;
%}

%%


.*"/*".*"*/" {
  /* Estado 0 para indicar que estamos dentro de comentarios */
  string  line_w_com, line_wo_com;
  int id_comment;
  line_w_com  = strdup(yytext);
  id_comment  = line_w_com.find("/*");
  line_wo_com = line_w_com.substr(0, id_comment);
  printf("%s", line_wo_com.c_str());
  break;
}
  /* Inicia comentario multilinea */
.*"/*".* {
  /* Estado 0 para indicar que estamos dentro de comentarios */
  state = 0;
  string  line_w_com, line_wo_com;
  int id_comment;
  line_w_com  = strdup(yytext);
  id_comment  = line_w_com.find("/*");
  line_wo_com = line_w_com.substr(0, id_comment);
  printf("%s", line_wo_com.c_str());
  break;
}
  /* Termina comentario multilinea */
.*"*/" {
  if(state == 0){
    state = 1;
  } else {
    printf("%s", yytext);
  }
  break;
}

  /* Inicia comentario una linea */
.*"//".*  {
  if(state == 1) {
  string line_w_com, line_wo_com;
  int id_comment;
  line_w_com  = strdup(yytext);
  id_comment  = line_w_com.find("//");
  line_wo_com = line_w_com.substr(0, id_comment);
  printf("%s", line_wo_com.c_str());
  break;
} else {
  break;
}
}
  /* Definición de macros */
"#define "[A-Z]+" ".* {
  string name, replacement, text, text_wo_define;
  int id_space;
  text           = strdup(yytext);
  text_wo_define = text.substr(8);
  id_space       = text_wo_define.find(" ");
  name           = text_wo_define.substr(0, id_space);
  replacement    = text_wo_define.substr(id_space);
  macros[name]   = replacement;
}
  /* Malas definiciones de macros */
"#define".* {
  /* printf("Mal macro\n"); */
  ReportError::InvalidDirective(number_line);
}

  /* Aplicación de macros */
"#"[A-Z]+ {
  string text, name, replacement;
	text = strdup(yytext);
	name = text.substr(1);
	if(macros.find(name) == macros.end()){
		ReportError::InvalidDirective(number_line);
    /* printf("Directiva invalida\n"); */
	} else {
		replacement = macros[name];
		printf("%s", replacement.c_str());
  }
}
  /* Todo lo demas */
.* {
  number_line++;
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
    /* printf("Error\n"); */
    return 1;
  } else{
    return 1;
  }
}
%%
