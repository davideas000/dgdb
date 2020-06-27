#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include "console_colors/console_colors.hpp"

//////////////////////////////////////////////////////////
/////////////////////// GdbManager////////////////////////
//////////////////////////////////////////////////////////

class GdbManager {

public:
  static const size_t buffer_size = 1000;

  GdbManager();
  virtual ~GdbManager();

  void init(const char* exe_file);
  void output() const;
  void command(const char* com, char gdb_out[]) const;

private:
  FILE* child_input;
  FILE* child_output;

};

//////////////////////////////////////////////////////////
////////////////////// gdb_outParser /////////////////////
//////////////////////////////////////////////////////////

class Parser {

public:
  static void parse(char* gdb_out, std::unordered_map<std::string, std::string>& result);
};

enum class Tag {equal_sign, eof, string, c_string, right_bracket, left_bracket, right_key,
quot};

class Lexer {

public:
  explicit Lexer(const std::string& input);
  std::pair<Tag, std::string> next_token() const;
  char next_char();

private:
  std::string buffer;
  size_t pos;

};

using namespace std;

//////////////////////////////////////////////////////////
////////////// GdbManager implementation /////////////////
//////////////////////////////////////////////////////////

GdbManager::GdbManager()
: child_input(NULL), child_output(NULL) {}

GdbManager::~GdbManager() {
  fclose(child_input);
  fclose(child_output);
}

void GdbManager::init(const char* exe_file) {

  int gdb_in[2];
  int gdb_out[2];

  pipe(gdb_in);
  pipe(gdb_out);

  switch (fork()) {
    case -1:
    perror("fork");
    exit(1);

    case 0:
    // child

    close(gdb_out[0]);
    dup2(gdb_out[1], STDOUT_FILENO);
    close(gdb_out[1]);

    close(gdb_in[1]);
    dup2(gdb_in[0], STDIN_FILENO);
    close(gdb_in[0]);

    execlp("gdb", "gdb", "-i", "mi", exe_file, (char*)NULL);
    perror("child execve");
    break;

    default:
    break;

  }

  // father
  close(gdb_in[0]);
  close(gdb_out[1]);

  child_input = fdopen(gdb_in[1], "w"); // write to gdb input
  child_output = fdopen(gdb_out[0], "r"); // read from gdb output

}

void GdbManager::output() const {

  // discovery how to read a whole line
  char gdb_out[buffer_size];

  while (1) {
    fgets(gdb_out, buffer_size, child_output);
    printf("%s", gdb_out);

    if (strcmp(gdb_out, "(gdb) \n") == 0) {
      break;
    }
  }

}

void GdbManager::command(const char* com, char gdb_out[]) const {

  fprintf(child_input, "%s\n", com);
  fflush(child_input);

  while (1) {
    fgets(gdb_out, buffer_size, child_output);

    if (gdb_out[0] == '^') {
      break;
    } else if(strcmp(gdb_out, "(gdb) \n") != 0) {
      if (gdb_out[0] == '*' || gdb_out[0] == '+' || gdb_out[0] == '=') {
        printf("%snotification: %s%s\n", DC_BLUE, gdb_out, DC_RESET);
      } else if (gdb_out[0] == '~' || gdb_out[0] == '&'){
        printf("%sstream output: %s%s\n", DC_MAGENTA, gdb_out, DC_RESET);
      } else {
        printf("%sprogram output: %s%s\n", DC_CYAN, gdb_out, DC_RESET);
      }
    }
  }
}

//////////////////////////////////////////////////////////
////////////// Parser implementation ///////////////
//////////////////////////////////////////////////////////

void Parser::parse(char* gdb_out, unordered_map<string, string>& result){
}

//////////////////////////////////////////////////////////
////////////// Lexer implementation ///////////////
//////////////////////////////////////////////////////////

Lexer::Lexer(const string& input)
: buffer(input), pos(0) {
}

pair<Tag, string> Lexer::next_token() const {
}

char Lexer::next_char() {
  return buffer[pos++];
}

int main(int argc, char const *argv[]) {
  puts("controling GDB");

  const char* exe_file = "build/extras/test1";

  GdbManager gdbm;
  gdbm.init(exe_file);

  gdbm.output();

  char gdb_out[GdbManager::buffer_size];
  gdbm.command("-break-insert main", gdb_out);
  printf("break insert gdb_out: %s\n", gdb_out);

  gdbm.command("-exec-run", gdb_out);
  printf("exec run gdb_out: %s\n", gdb_out);

  // gdb_com("-stack-list-variables --thread 1 --frame 0 --simple-values", gdb_out);
  gdbm.command("-stack-list-variables --simple-values", gdb_out);
  unordered_map<string, string> result;
  Parser::parse(gdb_out, result);
  printf("info locals gdb_out: %s\n", gdb_out);

  gdbm.command("-exec-continue", gdb_out);
  printf("exec run gdb_out: %s\n", gdb_out);

  gdbm.command("-gdb-exit", gdb_out);
  printf("gdb command gdb_out: %s\n", gdb_out);

  wait(NULL);
}
