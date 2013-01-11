#ifndef MATHEMATICA_MATHLINK_INTERFACE__HH
#define MATHEMATICA_MATHLINK_INTERFACE__HH

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

/*! @file  */

//! This namespace containts a self-contained C++ interface to Mathematica MathLink
/*  Nothing in here depends on libXon, it is completely independent.
 */
namespace mma {

class packet;
class mathlink;


//! Opaque pointer to mathlink's internal state
typedef void *MLINK;

//! Opaque pointer to mathlink's environment (not sure how it differs from MLINK)
typedef void *MLENV;


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
    //! Regular input prompt "In[..]:="
    INPUT_NAME = 8,
    //! Special input prompt: Input["..."]
    INPUT = 1,     
    //! Regular output prompt "Out[..]:="
    OUTPUT_NAME = 9, 
    //! Function call returning text
    RETURN_TEXT = 4,
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
    INTEGER = 0x2b,
    FUNCTION = 0x46,
  };
private:
  friend class mathlink;
  packet_header pkt_header;
  packet_type pkt_type;
  static packet* read_packet(MLINK ml);
  virtual void send_packet(MLINK ml) const;
protected:
  //! Count the number of UTF-8 characters
  int utf8len(const std::string s) const;
  //! Used when receiving a packet
  packet(packet_header header, MLINK ml);
  //! Used when constructing a packet to send
  packet(packet_type type);
public:
  virtual ~packet();
  packet_header header() const;
  packet_type type() const;
  bool is_valid() const;
  bool is_error() const;
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
  virtual std::string to_string() const;
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
  virtual std::string to_string() const;
};


//! Subtype of packet: a Mathematica function
/*  The arguments of the function are passed as subsequent packets 
 */
class packet_function : public packet
{
private:
  std::string fname;
  int n;
protected:
  friend class packet;
  packet_function(packet_header header, MLINK ml);
  virtual void send_packet(MLINK ml) const;
public:
  packet_function(std::string function_name, int number_of_arguments);
  virtual std::string to_string() const;
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
  virtual std::string to_string() const;
};


//! MathLink environment
/*  Not really interesting for users, no idea what the ML environment
 *  is good for. But RAII dictates that it must be a class, so here
 *  it is.
 */
class environment
{
private:
  friend class mathlink;
  MLENV ml_env;
  environment();
  void initialize();
public:
  virtual ~environment();
};


//! Mathlink session interfacing the Mathematica kernel
class mathlink
{
private:
  environment env;
  MLINK ml;
  long interface_version, revision_version;
  //! The mathematica root directory, e.g. "/usr/local/Wolfram/Mathematica/9.0"
  static void* library_handle;
  std::string library_name;
  void search_mathlink(const std::string& root_dir);
  void dlopen_mathlink(const std::string& shared_library);
  void dlopen_mathlink_dependency(const std::string& shared_library) const;
  std::vector<std::string> list_dir(const std::string& dir, bool files, bool directories) const;
protected:
  void print_error();
  //! Open the MathLink interface
  void open(const std::string& mathematica_command);
  void close();
public:
  //! Start a MathLink session
  /*  @param mathematica_root_directory the directory where
   *         Mathematica is installed, for example
   *         "/usr/local/Wolfram/Mathematica/9.0". This is the
   *         $InstallationDirectory variable in Mathematica.
   */
  mathlink(const std::string& mathematica_root_directory);
  //! Start a MathLink session by explicitly providing the shared library
  /*  @param mathematica_command the command to run Mathematica text
   *         mode, usually "math". Use an absolute path name if the
   *         command is not in the system $PATH.
   *  @param mathlink_shared_library the shared library for mathlink,
   *         for example
   *         "/usr/local/Wolfram/Mathematica/9.0/SystemFiles/Links/MathLink/DeveloperKit/Linux-x86-64/CompilerAdditions/libML64i3.so".
   *         Must be a full path name unless it is in the system
   *         library search path, which is not where Mathematica
   *         installs it.
   */
  mathlink(const std::string& mathematica_command, const std::string& mathlink_shared_library);
  virtual ~mathlink();
  long interface() const;
  long revision() const;
  const std::string& shared_library() const;
  void send(const packet& pkt);
  packet* receive();
};


//! Any exception during Mathematica Mathlink operation
class mathlink_exception : public std::runtime_error
{
public:
  mathlink_exception(const std::string& what) : std::runtime_error(what) {};
};


} // end namespace mma

#endif /* MATHEMATICA_MATHLINK_INTERFACE__HH */
