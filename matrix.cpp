#include <string>
using std::string;

struct R
{
  string type;
  short op;
  short rs;
  short rt;
  short rd;
  short shemt;
  short funct;
};

struct I
{
  string type;
  short op;
  short rs;
  short rt;
  int address;
};

struct J {
  string type;
  short op;
  int address;
};

struct Label {
  string label;
  short line;
};

// Label l = {"L1", 1};
