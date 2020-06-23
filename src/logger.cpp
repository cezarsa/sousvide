#include <HardwareSerial.h>
#include <Syslog.h>
#include <WiFiUdp.h>

const size_t maxLineSize = 1024;

class loggerPrint : public Print {
  WiFiUDP udpClient;
  Syslog syslog;
  char lineBuffer[maxLineSize];
  size_t pos = 0;

 public:
  loggerPrint()
      : udpClient(),
        syslog(udpClient,
               "255.255.255.255",
               514,
               "sousvide",
               "-",
               LOG_MAKEPRI(LOG_KERN, LOG_INFO)){};

  void flushBuffer() {
    if (pos == 0) {
      return;
    }
    lineBuffer[pos] = 0;
    pos = 0;
    syslog.log(lineBuffer);
    Serial.println(lineBuffer);
  }

  size_t write(uint8_t byte) override {
    if (byte == '\n' || byte == '\r') {
      flushBuffer();
      return 1;
    }
    lineBuffer[pos] = byte;
    pos++;
    if (pos == maxLineSize - 1) {
      flushBuffer();
    }
    return 1;
  }
};

loggerPrint syslogLogger;
loggerPrint& logger(syslogLogger);
