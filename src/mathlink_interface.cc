#include <cassert>
#include <sstream>
#include <stdint.h>
#include <dlfcn.h>

#include "mathlink_interface.hh"

// We assume that MLINTERFACE >= 3. This version changed the API to
// return standard C types for a modicum of sanity

#define EXTERN_C_BEGIN  extern "C" {
#define EXTERN_C_END    }

namespace MmaMathLink {


typedef MLENV (*MLInitialize_func)(char *p);
typedef void (*MLDeinitialize_func)(MLENV env);
typedef MLINK (*MLOpenArgv_func)(MLENV env, const char **argv_from, const char **argv_to, int *error);
typedef int (*MLActivate_func)(MLINK ml);
typedef void (*MLVersionNumber0_func)(MLENV env, long *interface_number, long *revision_number);

typedef void (*MLClose_func)(MLINK ml);

typedef int (*MLError_func)(MLINK ml);
typedef int (*MLClearError_func)(MLINK ml);
typedef const char* (*MLErrorMessage_func)(MLINK ml);

typedef int (*MLGetType_func)(MLINK ml);
typedef int (*MLNextPacket_func)(MLINK ml);

typedef int (*MLGetSymbol_func)(MLINK ml, char **s);
typedef void (*MLReleaseSymbol_func)(MLINK ml, char *s);
typedef int (*MLPutSymbol_func)(MLINK ml, const char *s);

typedef int (*MLGetUTF8Symbol_func)(MLINK ml, char **s, int *nbytes, int *nchars);
typedef void (*MLReleaseUTF8Symbol_func)(MLINK ml, char *s, int nbytes);
typedef int (*MLPutUTF8Symbol_func)(MLINK ml, const char *s, int nbytes);

typedef int (*MLGetString_func)(MLINK ml, char **s);
typedef void (*MLReleaseString_func)(MLINK mlp, char *s);
typedef int (*MLPutString_func)(MLINK ml, const char *s);

typedef int (*MLGetUTF8String_func)(MLINK ml, char **s, int *nbytes, int *nchars);
typedef void (*MLReleaseUTF8String_func)(MLINK mlp, char *s, int nbytes);
typedef int (*MLPutUTF8String_func)(MLINK ml, const char *s, int nbytes);

typedef int (*MLGetInteger32_func)(MLINK ml, int32_t *value);
typedef int (*MLGetInteger64_func)(MLINK ml, int64_t *value);

typedef int (*MLGetReal32_func)(MLINK ml, float *value);
typedef int (*MLGetReal64_func)(MLINK ml, double *value);

typedef int (*MLPutFunction_func)(MLINK ml, const char *function_name, int number_of_arguments);

typedef int (*MLEndPacket_func)(MLINK ml);


typedef struct {
  MLInitialize_func Initialize;
  MLDeinitialize_func Deinitialize;

  MLOpenArgv_func OpenArgv;
  MLActivate_func Activate;
  MLVersionNumber0_func VersionNumber0;
  
  MLClose_func Close;
  
  MLError_func Error;
  MLClearError_func ClearError;
  MLErrorMessage_func ErrorMessage;
  
  MLGetType_func GetType;
  MLNextPacket_func NextPacket;
  
  MLGetSymbol_func GetSymbol;
  MLReleaseSymbol_func ReleaseSymbol;
  MLPutSymbol_func PutSymbol;
  
  MLGetUTF8Symbol_func GetUTF8Symbol;
  MLReleaseUTF8Symbol_func ReleaseUTF8Symbol;
  MLPutUTF8Symbol_func PutUTF8Symbol;
  
  MLGetString_func GetString;
  MLReleaseString_func ReleaseString;
  MLPutString_func PutString;
  
  MLGetUTF8String_func GetUTF8String;
  MLReleaseUTF8String_func ReleaseUTF8String;
  MLPutUTF8String_func PutUTF8String;
  
  MLGetInteger32_func GetInteger32;
  MLGetInteger64_func GetInteger64;
  
  MLGetReal32_func GetReal32;
  MLGetReal64_func GetReal64;
  
  MLPutFunction_func PutFunction;
  
  MLEndPacket_func EndPacket;
} MathLinkInterface;


static MathLinkInterface ML;


/////////////////////////////////////////////////////////
///
///  Implementation: class environment
///
////////////////////////////////////////////////////////

environment::environment() 
{
  ml_env = ML.Initialize(NULL);
  if (ml_env == NULL)
    throw mathlink_exception("initialization failed");
}


environment::~environment() 
{
  ML.Deinitialize(ml_env);
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
  if (ML.Error(ml))
    cerr << "Error: " << ML.ErrorMessage(ml) << endl;
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
  ml = ML.OpenArgv(env.ml_env, nargv, nargv+sizeof(nargv)/sizeof(*nargv)-1, &error);
  std::cout << "error = " << error << std::endl;
  if (ml == NULL) {
    print_error();
    throw mathlink_exception("opening link failed");
  }

  int rc = ML.Activate(ml);
  if (!rc) {
    print_error();
    throw mathlink_exception("connecting failed");
  }

  ML.VersionNumber0(env.ml_env, &interface_version, &revision_version);
  std::cout << "MathLink version " << interface_version << " - " << revision_version << std::endl;
}

void mathlink::close()
{
  ML.Close(ml);
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

void* mathlink::library_handle = NULL;


void mathlink::dlopen_mathlink(const std::string shared_library) const
{
  library_handle = dlopen(shared_library.c_str(), RTLD_LAZY);
  if (library_handle == NULL)
    throw mathlink_exception("opening shared library failed");
  
  ML.Initialize    = (MLInitialize_func)dlsym(library_handle, "MLInitialize");
  ML.Deinitialize  = (MLDeinitialize_func)dlsym(library_handle, "Deinitialize");

  ML.OpenArgv      = (MLOpenArgv_func)dlsym(library_handle, "OpenArgv");
  ML.Activate      = (MLActivate_func)dlsym(library_handle, "Activate");
  ML.VersionNumber0 = (MLVersionNumber0_func)dlsym(library_handle, "VersionNumber0");
  ML.Close         = (MLClose_func)dlsym(library_handle, "Close");

  ML.Error         = (MLError_func)dlsym(library_handle, "Error");
  ML.ClearError    = (MLClearError_func)dlsym(library_handle, "ClearError");
  ML.ErrorMessage  = (MLErrorMessage_func)dlsym(library_handle, "ErrorMessage");
  
  ML.GetType       = (MLGetType_func)dlsym(library_handle, "GetType");
  ML.NextPacket    = (MLNextPacket_func)dlsym(library_handle, "NextPacket");
  
  ML.GetSymbol     = (MLGetSymbol_func)dlsym(library_handle, "GetSymbol");
  ML.ReleaseSymbol = (MLReleaseSymbol_func)dlsym(library_handle, "ReleaseSymbol");
  ML.PutSymbol     = (MLPutSymbol_func)dlsym(library_handle, "PutSymbol");
  
  ML.GetUTF8Symbol = (MLGetUTF8Symbol_func)dlsym(library_handle, "GetUTF8Symbol");
  ML.ReleaseUTF8Symbol = (MLReleaseUTF8Symbol_func)dlsym(library_handle, "ReleaseUTF8Symbol");
  ML.PutUTF8Symbol = (MLPutUTF8Symbol_func)dlsym(library_handle, "PutUTF8Symbol");
  
  ML.GetString     = (MLGetString_func)dlsym(library_handle, "GetString");
  ML.ReleaseString = (MLReleaseString_func)dlsym(library_handle, "ReleaseString");
  ML.PutString     = (MLPutString_func)dlsym(library_handle, "PutString");
  
  ML.GetUTF8String = (MLGetUTF8String_func)dlsym(library_handle, "GetUTF8String");
  ML.ReleaseUTF8String = (MLReleaseUTF8String_func)dlsym(library_handle, "ReleaseUTF8String");
  ML.PutUTF8String = (MLPutUTF8String_func)dlsym(library_handle, "PutUTF8String");
  
  ML.GetInteger32  = (MLGetInteger32_func)dlsym(library_handle, "GetInteger32");
  ML.GetInteger64  = (MLGetInteger64_func)dlsym(library_handle, "GetInteger64");
  
  ML.GetReal32     = (MLGetReal32_func)dlsym(library_handle, "GetReal32");
  ML.GetReal64     = (MLGetReal64_func)dlsym(library_handle, "GetReal64");
  
  ML.PutFunction   = (MLPutFunction_func)dlsym(library_handle, "PutFunction");
  
  ML.EndPacket     = (MLEndPacket_func)dlsym(library_handle, "EndPacket");
}


void mathlink::dlopen_mathlink() const
{
  std::string dir = root_dir + "/SystemFiles/Links/MathLink/DeveloperKit";
  dlopen_mathlink(dir + "/Linux-x86-64/CompilerAdditions/libML64i3.so");
}


/////////////////////////////////////////////////////////
///
///  Implementation: class packet
///
////////////////////////////////////////////////////////

packet* packet::read_packet(MLINK ml)
{
  packet_header header = packet_header(ML.NextPacket(ml));
  packet_type type = packet_type(ML.GetType(ml));
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

  // std::cout << "ML.NextPacket returned " 
  // 	    << header << " " 
  // 	    << std::hex << type 
  // 	    << " " << pkt->to_string()
  // 	    << std::endl;
  
  return pkt;
}

packet::packet(packet_header header, MLINK ml)
  : pkt_header(header)
{
  pkt_type = packet_type(ML.GetType(ml));
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
  if (!ML.GetUTF8Symbol(ml, &ch, &nbytes, &nchars))
    throw mathlink_exception("illegal symbol");
  str = ch;
  ML.ReleaseUTF8Symbol(ml, ch, nbytes);
}

std::string packet_symbol::to_string() const 
{ 
  return str; 
}

void packet_symbol::send_packet(MLINK ml) const
{
  int rc = ML.PutUTF8Symbol(ml, str.c_str(), utf8len(str));
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
  if (!ML.GetUTF8String(ml, &ch, &nbytes, &nchars))
    throw mathlink_exception("illegal string");
  str = ch;
  ML.ReleaseUTF8String(ml, ch, nchars);
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
  int rc = ML.PutUTF8String(ml, str.c_str(), utf8len(str));
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
  int rc = ML.PutFunction(ml, fname.c_str(), n);
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
  int rc = ML.EndPacket(ml);
  if (!rc)
    throw mathlink_exception("illegal end packet");
}




} // end namespace MmaMathLink
