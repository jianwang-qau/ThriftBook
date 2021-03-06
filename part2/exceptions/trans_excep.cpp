// Apache Thrift Transport Exceptions in C++

#include <thrift/Thrift.h>
#include <thrift/transport/TTransportException.h>
#include <thrift/transport/TSimpleFileTransport.h>
#include <iostream>
#include <exception>					
#include <memory>
#include <cstring>

using namespace apache::thrift::transport;

struct Trade {
   char symbol[16];
   double price;
   int size;
};

int main(int argc, char ** argv)
{
   try {
      std::unique_ptr<TTransport> trans;
      if (argc > 1)					
        trans.reset(new TSimpleFileTransport("data", false, false));
      else
        trans.reset(new TSimpleFileTransport("data", false, true));
      Trade trade{"F", 13.10, 2500};
      trans->write((const uint8_t *)&trade, sizeof(trade));
      trans->close();

      trans.reset(new TSimpleFileTransport("data",true,false));
      std::memset(&trade, 0, sizeof(trade));
      int bytes_read = trans->read((uint8_t *)&trade,sizeof(trade));

      std::cout << "Trade(" << bytes_read << "): " << trade.symbol 
                << " " << trade.size << " @ " << trade.price 
                << std::endl;
   } catch (const apache::thrift::transport::TTransportException & tte) {
       std::cout << "TTransportException(" << tte.getType() << "): " 
 		         << tte.what() << std::endl;
   } catch (const apache::thrift::TException & te) {			
       std::cout << "TException: " << te.what() << std::endl;
   } catch (const std::exception & e) {					
       std::cout << "exception: " << e.what() << std::endl;
   } catch (...) {								
       std::cout << "Unknown Exception" << std::endl;
   }
}

