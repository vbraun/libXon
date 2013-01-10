#include <cassert>
#include <iostream>
#include <string>

#include "xon/support.h"
#include "xon/server.hh"
#include "xon/exceptions.hh"
#include "debug.h"

// We assume that MLINTERFACE >= 3. This version changed the API to
// return standard C types for a modicum of sanity

XON__EXTERN_C_BEGIN

//! Opaque pointer to mathlink's internal state
typedef void *MLINK;

//! Opaque pointer to mathlink's environment (not sure how it differs from MLINK)
typedef void *MLEnvironment;

MLEnvironment MLInitialize(char *p);
MLINK MLOpenArgv(MLEnvironment env, const char **argv_from, const char **argv_to, int *error);
int MLActivate(MLINK ml);
void MLVersionNumber0(MLEnvironment env, long *interface_number, long *revision_number);

void MLClose(MLINK ml);

int MLError(MLINK ml);
int MLClearError(MLINK ml);
const char *MLErrorMessage(MLINK ml);

int MLGetType(MLINK ml);
int MLNextPacket(MLINK ml);

int MLGetSymbol(MLINK ml, char **s);
void MLReleaseSymbol(MLINK ml, char *s);

int MLGetString(MLINK ml, char **s);
void MLReleaseString(MLINK mlp, char *s);

int MLGetInteger32(MLINK ml, int32_t *value);
int MLGetInteger64(MLINK ml, int64_t *value);

int MLGetReal32(MLINK ml, float *value);
int MLGetReal64(MLINK ml, double *value);

XON__EXTERN_C_END


class packet;
class mathlink;

/////////////////////////////////////////////////////////
///
///  Declarations
///
////////////////////////////////////////////////////////

//! One Mathlink packet
class packet
{
public:
  enum packet_header {   // the foobarPKT constants
    //! We constructed packet manually
    CONSTRUCTED = -1,  
    ILLEGAL = 0,
    CALL = 7,
    EVALUATE = 13,
    RETURN = 3,
    //! Regular prompt "In[..]:="
    INPUT_NAME = 8,
    //! Special input prompt: Input["..."]
    INPUT = 1,     
  };
  enum packet_type {     // the MLTKfoobar constants
    //! Not part of the MathLink API, used internally
    END_MARKER = -1,  
    //! An error has occurred
    ERROR = 0x00,
    //! Packet is a string
    STRING = 0x22,
    SYMBOL = 0x23,
    REAL = 0x2a,
    INT = 0x2b,
    FUNCTION = 0x46,
  };
private:
  friend class mathlink;
  packet_header pkt_header;
  packet_type pkt_type;
  static packet* read_packet(MLINK ml);
  virtual void send_packet(MLINK ml) const;
protected:
  //! Used when receiving a packet
  packet(packet_header header, MLINK ml);
  //! Used when constructing a packet to send
  packet(packet_type type);
public:
  ~packet();
  packet_header header() const;
  packet_type type() const;
  bool is_valid() const;
  virtual std::string to_string() const;
  friend std::ostream& operator << (std::ostream& out, const packet& pkt);
};


//! Subtype of packet: symbol (e.g. input prompt)
class packet_symbol : public packet
{
private:
  std::string str;
protected:
  friend class packet;
  packet_symbol(packet_header header, MLINK ml);
  virtual void send_packet(MLINK ml) const;
public:
  virtual std::string to_string() const { return str; };
};


//! Subtype of packet: string
class packet_string : public packet
{
private:
  std::string str;
protected:
  friend class packet;
  packet_string(packet_header header, MLINK ml);
  virtual void send_packet(MLINK ml) const;
public:
  packet_string(std::string string);
  virtual std::string to_string() const { return str; };
};


//! Subtype of packet: a Mathematica function
class packet_function : public packet
{
private:
  std::string fname;
protected:
  friend class packet;
  packet_function(packet_header header, MLINK ml);
  virtual void send_packet(MLINK ml) const;
public:
  packet_function(std::string function_name);
  virtual std::string to_string() const { return fname; };
};

//! Subtype of packet: end of input stream
class packet_end : public packet
{
protected:
  friend class packet;
  packet_end(packet_header header, MLINK ml);
  virtual void send_packet(MLINK ml) const;
public:
  packet_end();
  virtual std::string to_string() const { return "end marker"; };
};


//! Mathlink session interfacing the Mathematica kernel
class mathlink
{
private:
  MLINK ml;
  long interface_version, revision_version;
protected:
  void error(MLINK ml);
  void open();
  void close();
public:
  mathlink();
  virtual ~mathlink();
  void send(const packet& pkt);
  packet* receive();
};


//! Any exception during Mathematica Mathlink operation
class mathlink_exception : public std::runtime_error
{
public:
  mathlink_exception(const std::string& what) : std::runtime_error(what) {};
};


/////////////////////////////////////////////////////////
///
///  Implementation: class mathlink
///
////////////////////////////////////////////////////////

mathlink::mathlink()
{
  open();
}

mathlink::~mathlink()
{
  close();
}


void mathlink::error()
{
  using namespace std;
  if (MLError(ml))
    cerr << "Error: " << MLErrorMessage(ml) << endl;
  else
    cerr << "Error is not error?!?" << endl;
}

void mathlink::open()
{
  MLEnvironment env = MLInitialize(NULL);
  if (env == NULL)
    throw mathlink_exception("initialization failed");

  const char *nargv[] = {
    "-linkmode", 
    "launch", 
    "-linkname", 
    "math", /*math_program+linkname[]+argv[]*/
    NULL};

  int error;
  ml = MLOpenArgv(env, nargv, nargv+sizeof(nargv)/sizeof(*nargv)-1, &error);
  std::cout << "error = " << error << std::endl;
  if (ml == NULL)
    throw mathlink_exception("opening link failed");

  int rc = MLActivate(ml);
  if (!rc)
    throw mathlink_exception("connecting failed");

  MLVersionNumber0(env, &interface_version, &revision_version);
  std::cout << "MathLink version " << interface_version << " - " << revision_version << std::endl;
}

void mathlink::close()
{
  MLClose(ml);
}


packet* mathlink::receive()
{
  return packet::read_packet(ml);
}

void mathlink::send(const packet& pkt) 
{
  pkt.send_packet(ml);
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
  case SYMBOL:
    pkt = new packet(header, ml);
    error();
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

  std::cout << "MLNextPacket returned " 
	    << header << " " 
	    << std::hex << type 
	    << " " << pkt->to_string()
	    << std::endl;
  
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
  out << "packet(0x" << std::hex << pkt.type()
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
  MLGetSymbol(ml, &ch);
  str = ch;
  MLReleaseSymbol(ml, ch);
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
  MLGetString(ml, &ch);
  str = ch;
  MLReleaseString(ml, ch);
}

packet_string::packet_string(std::string string)
  : packet(packet::STRING), str(string)
{
}


/////////////////////////////////////////////////////////
///
///  Implementation: class packet_function
///
////////////////////////////////////////////////////////

packet_function::packet_function(packet_header header, MLINK ml)
  : packet(header, ml)
{
}

packet_function::packet_function(std::string function_name)
  : packet(packet::FUNCTION), fname(function_name)
{
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


/////////////////////////////////////////////////////////
///
///  Main loop
///
////////////////////////////////////////////////////////

int main(void)
{
  using namespace std;
  mathlink ml;
  packet *pkt;

  pkt = ml.receive();
  assert(pkt->header() == packet::INPUT_NAME);
  cout << *pkt;
  delete pkt;

  ml.send(packet_function("EnterTextPacket"));
  ml.send(packet_string("1+1"));
  ml.send(packet_end());

  pkt = ml.receive();
  cout << *pkt;
  delete pkt;

  pkt = ml.receive();
  cout << *pkt;
  delete pkt;

  return 0;
}
