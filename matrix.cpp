#include <string>
using std::string;

struct R
{
  string type;
  short funct;
  short op{};
};

struct I
{
  string type;
  short op;
};

struct J {
  string type;
  short op;
};

struct Label {
  string label;
  short line;
};

string registradores[32] = { "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$s8","$ra" };

R r[18] = { {"sll", 0}, {"srl", 2}, {"jr", 8}, {"mtfhi", 16}, {"mthflo", 18}, {"mult", 24}, {"multu", 25},{ "div", 26},{ "divu", 27}, {"add", 32}, {"addu", 33}, {"sub", 34}, {"subu", 35}, {"and", 36},{ "or", 37},{ "slt", 43}, {"sltu", 43}, {"mul", 2, 28} };
I i[11]{};
J j[2]{};

bool isI(string str) {
  for(int k = 0; k < 11; k++){
    if(str == i[k].type)
      return true;
  }
  return false;
}

bool isJ(string str){
  for(int i = 0; i < 2; i++){
    if(str == j[i].type)
      return true;
  }
  return false;
}
    