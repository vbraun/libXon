#include <cassert>
#include <sstream>
#include <stdint.h>

#include "mathlink_interface.hh"
#include "path.hh"

// We assume that MLINTERFACE >= 3. This version changed the API to
// return standard C types for a modicum of sanity

#define EXTERN_C_BEGIN  extern "C" {
#define EXTERN_C_END    }

namespace MmaMathLink {


EXTERN_C_BEGIN

MLENV MLInitialize(char *p);
void MLDeinitialize(MLENV env);
MLINK MLOpenArgv(MLENV env, const char **argv_from, const char **argv_to, int *error);
int MLActivate(MLINK ml);
void MLVersionNumber0(MLENV env, long *interface_number, long *revision_number);

void MLClose(MLINK ml);

int MLError(MLINK ml);
int MLClearError(MLINK ml);
const char *MLErrorMessage(MLINK ml);

int MLGetType(MLINK ml);
int MLNextPacket(MLINK ml);

int MLGetSymbol(MLINK ml, char **s);
void MLReleaseSymbol(MLINK ml, char *s);
int MLPutSymbol(MLINK ml, const char *s);

int MLGetUTF8Symbol(MLINK ml, char **s, int *nbytes, int *nchars);
void MLReleaseUTF8Symbol(MLINK ml, char *s, int nbytes);
int MLPutUTF8Symbol(MLINK ml, const char *s, int nbytes);

int MLGetString(MLINK ml, char **s);
void MLReleaseString(MLINK mlp, char *s);
int MLPutString(MLINK ml, const char *s);

int MLGetUTF8String(MLINK ml, char **s, int *nbytes, int *nchars);
void MLReleaseUTF8String(MLINK mlp, char *s, int nbytes);
int MLPutUTF8String(MLINK ml, const char *s, int nbytes);

int MLGetInteger32(MLINK ml, int32_t *value);
int MLGetInteger64(MLINK ml, int64_t *value);

int MLGetReal32(MLINK ml, float *value);
int MLGetReal64(MLINK ml, double *value);

int MLPutFunction(MLINK ml, const char *function_name, int number_of_arguments);

int MLEndPacket(MLINK ml);

EXTERN_C_END




/////////////////////////////////////////////////////////
///
///  Implementation: class environment
///
////////////////////////////////////////////////////////

environment::environment() 
{
  ml_env = MLInitialize(NULL);
  if (ml_env == NULL)
    throw mathlink_exception("initialization failed");
}


environment::~environment() 
{
  MLDeinitialize(ml_env);
}


/////////////////////////////////////////////////////////
///
///  Implementation: class mathlink
///
////////////////////////////////////////////////////////

mathlink::mathlink(const std::string& mathematica_root_directory)
  : root_dir(mathematica_root_directory)
{
  dlopen_mathlink();
  open();
}

mathlink::~mathlink()
{
  close();
}


void mathlink::print_error()
{
  using namespace std;
  if (MLError(ml))
    cerr << "Error: " << MLErrorMessage(ml) << endl;
  else
    cerr << "Error is not error?!?" << endl;
}

void mathlink::open()
{
  const std::string command = root_dir + "/Executables/math";

  const char *nargv[] = {
    "-linkmode", 
    "launch", 
    "-linkname", 
    command.c_str(), /*math_program+linkname[]+argv[]*/
    NULL};

  int error;
  ml = MLOpenArgv(env.ml_env, nargv, nargv+sizeof(nargv)/sizeof(*nargv)-1, &error);
  std::cout << "error = " << error << std::endl;
  if (ml == NULL) {
    print_error();
    throw mathlink_exception("opening link failed");
  }

  int rc = MLActivate(ml);
  if (!rc) {
    print_error();
    throw mathlink_exception("connecting failed");
  }

  MLVersionNumber0(env.ml_env, &interface_version, &revision_version);
  std::cout << "MathLink version " << interface_version << " - " << revision_version << std::endl;
}

void mathlink::close()
{
  MLClose(ml);
}



packet* mathlink::receive()
{
  packet *pkt = packet::read_packet(ml);
  return pkt;
}

void mathlink::send(const packet& pkt) 
{
  try {
    pkt.send_packet(ml);
  } catch (mathlink_exception& e) {
    print_error();
    throw;
  }
}

long mathlink::interface() const
{
  return interface_version;
}

long mathlink::revision() const
{
  return revision_version;
}

void mathlink::dlopen_mathlink(const std::string shared_library) const
{
}

void mathlink::dlopen_mathlink() const
{
  path p = root_dir + "/SystemFiles/Links/MathLink/DeveloperKit";
}


/////////////////////////////////////////////////////////
///
///  Implementation: class packet
///
////////////////////////////////////////////////////////

packet* packet::read_packet(MLINK ml)
{
  packet_header header = packet_header(MLNextPacket(ml));
  packet_type type = packet_type(MLGetType(ml));
  packet *pkt;
  
  switch (type) {
  case ERROR:
    pkt = new packet(header, ml);
    break;
  case SYMBOL:
    pkt = new packet_symbol(header, ml);
    break;
  case STRING:
    pkt = new packet_string(header, ml);
    break;
  case FUNCTION:
    pkt = new packet_function(header, ml);
    break;
  default:
    pkt = new packet(header, ml);
    break;
  }

  // std::cout << "MLNextPacket returned " 
  // 	    << header << " " 
  // 	    << std::hex << type 
  // 	    << " " << pkt->to_string()
  // 	    << std::endl;
  
  return pkt;
}

packet::packet(packet_header header, MLINK ml)
  : pkt_header(header)
{
  pkt_type = packet_type(MLGetType(ml));
}

packet::packet(packet_type type)
  : pkt_header(CONSTRUCTED), pkt_type(type)
{
}

packet::~packet()
{
}

int packet::utf8len(const std::string str) const
{
  int length = 0;
  const char *s = str.c_str();
  while (*s) // count all binary 10******
    length += (*s++ & 0xc0) != 0x80;
  return length;
}

packet::packet_header packet::header() const
{
  return this->pkt_header;
}

packet::packet_type packet::type() const
{
  return this->pkt_type;
}

bool packet::is_valid() const
{
  return header() != ILLEGAL;
}

bool packet::is_error() const
{
  return type() == ERROR;
}

std::string packet::to_string() const
{
  return std::string("undefined");
}

void packet::send_packet(MLINK ml) const
{
  throw mathlink_exception("cannot send base type packet");
}

std::ostream& operator << (std::ostream& out, const packet& pkt)
{
  out << "packet(" << pkt.header()
      << ", 0x" << std::hex << pkt.type()
      << ", " << pkt.to_string() << ")" << std::endl;
  return out;
}


/////////////////////////////////////////////////////////
///
///  Implementation: class packet_symbol
///
////////////////////////////////////////////////////////

packet_symbol::packet_symbol(packet_header header, MLINK ml)
  : packet(header, ml)
{
  char *ch;
  int nbytes, nchars;
  if (!MLGetUTF8Symbol(ml, &ch, &nbytes, &nchars))
    throw mathlink_exception("illegal symbol");
  str = ch;
  MLReleaseUTF8Symbol(ml, ch, nbytes);
}

std::string packet_symbol::to_string() const 
{ 
  return str; 
}

void packet_symbol::send_packet(MLINK ml) const
{
  int rc = MLPutUTF8Symbol(ml, str.c_str(), utf8len(str));
  if (!rc)
    throw mathlink_exception("illegal symbol");
}


/////////////////////////////////////////////////////////
///
///  Implementation: class packet_string
///
////////////////////////////////////////////////////////

packet_string::packet_string(packet_header header, MLINK ml)
  : packet(header, ml)
{
  char *ch;
  int nbytes, nchars;
  if (!MLGetUTF8String(ml, &ch, &nbytes, &nchars))
    throw mathlink_exception("illegal string");
  str = ch;
  MLReleaseUTF8String(ml, ch, nchars);
}

packet_string::packet_string(std::string string)
  : packet(packet::STRING), str(string)
{
}

std::string packet_string::to_string() const 
{ 
  return str; 
}

void packet_string::send_packet(MLINK ml) const
{
  int rc = MLPutUTF8String(ml, str.c_str(), utf8len(str));
  if (!rc)
    throw mathlink_exception("illegal string");
}


/////////////////////////////////////////////////////////
///
///  Implementation: class packet_function
///
////////////////////////////////////////////////////////

packet_function::packet_function(packet_header header, MLINK ml)
  : packet(header, ml)
{
  throw mathlink_exception("function packet can only be sent, not received");
}

packet_function::packet_function(std::string function_name, int number_of_arguments)
  : packet(packet::FUNCTION), fname(function_name), n(number_of_arguments)
{
}

std::string packet_function::to_string() const 
{ 
  std::stringstream ss;
  ss << fname << "+" << n;
  return ss.str();
}

void packet_function::send_packet(MLINK ml) const
{
  int rc = MLPutFunction(ml, fname.c_str(), n);
  if (!rc)
    throw mathlink_exception("illegal fuction");
}

/////////////////////////////////////////////////////////
///
///  Implementation: class packet_end
///
////////////////////////////////////////////////////////

packet_end::packet_end(packet_header header, MLINK ml)
  : packet(header, ml)
{
  throw mathlink_exception("packet end marker can only be sent, not received");
}

packet_end::packet_end()
  : packet(packet::FUNCTION)
{
}

std::string packet_end::to_string() const 
{ 
  return "end marker"; 
}

void packet_end::send_packet(MLINK ml) const
{
  int rc = MLEndPacket(ml);
  if (!rc)
    throw mathlink_exception("illegal end packet");
}




} // end namespace MmaMathLink
