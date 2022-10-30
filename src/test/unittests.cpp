/* 
 * **************************************************************************
 * 
 *  file:       unittests.cpp
 *  project:    LCGateway
 *  subproject: main application
 *  purpose:    network gateway to linuxcnc nml interface         
 *  created:    25.10.2022 by Django Reinhard
 *  copyright:  (c) 2022 Django Reinhard -  all rights reserved
 * 
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 2 of the License, or 
 *  (at your option) any later version. 
 *   
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 *   
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * **************************************************************************
 */
#include <unittests.h>
#include <authenticator.h>
#include <httprequest.h>
#include <httpresponse.h>
#include <httperror.h>
#include <testclient.h>
#include <QHostAddress>
#include <QTcpSocket>
#include <QDebug>
#include <iostream>
#include <emc_nml.hh>


void UnitTests::testRequest() {
  QSKIP("not now");
  HttpRequest r("/fasel/blub", "keine Ahnung");
  QString test(r.toByteArray());
  QStringList parts = test.split("\r\n");

  std::cout << "\ntest-request:" << std::endl << test.toStdString().c_str() << std::endl << std::endl;

  QCOMPARE(parts.at(0), QStringLiteral("POST /fasel/blub HTTP/1.1"));
  QCOMPARE(r.headerValue("Content-Length"), "12");

  r.setPayLoad("jetzt geht es ab! Mann!");
  test = r.toByteArray();

  std::cout << "\ntest-request:" << std::endl << test.toStdString().c_str() << std::endl << std::endl;

  QCOMPARE(r.headerValue("Content-Length"), "23");
  }


void UnitTests::testResponse() {
  QSKIP("not now");
  HttpResponse r;

  r.setHeaderValue(QStringLiteral("Realm"), "what-ever");
  r.setPayLoad("no body at all");
  QString test(r.toByteArray());
  QStringList parts = test.split("\r\n");

//  std::cout << "\ntest-response:" << std::endl << test.toStdString().c_str() << std::endl << std::endl;

  QCOMPARE(parts.at(0), QStringLiteral("HTTP/1.1 200 OK"));
  }


void UnitTests::testError() {
  QSKIP("not now");
  HttpError nf(ResponseCode::NotFound, "big master");
  QString test(nf.toByteArray());
  QStringList parts = test.split("\r\n");

//  std::cout << "\ntest-error:" << std::endl << test.toStdString().c_str() << std::endl << std::endl;

  QCOMPARE(parts.at(0), QStringLiteral("HTTP/1.1 404 Not found"));
  }


void UnitTests::testHostAddress() {
  QSKIP("not now");
  QHostAddress  addr("192.16.13.99");

  qDebug() << "host-address:" << addr;
  }


void UnitTests::testRequestParsing() {
//  QSKIP("not now");
  QString src("POST /linuxcnc/command HTTP/1.1\r\n"
              "Accept: */*\r\n"
              "Connection: keep-alive\r\n"
              "Content-Type: text/plain\r\n"
              "Realm: 573A0EAAA2AE57E7806B233134DE4F2B\r\n"
              "Content-Length: 32\r\n"
              "\r\n"
              "-QEAAAAAAAAYAAAAAAAAAAAAAAAEAAAA");
  QString chk("POST /linuxcnc/command HTTP/1.1\r\n"
              "Accept: */*\r\n"
              "Connection: keep-alive\r\n"
              "Content-Type: text/plain\r\n"
              "Realm: 573A0EAAA2AE57E7806B233134DE4F2B\r\n"
              "Content-Length: 32\r\n"
              "\r\n"
              "-QEAAAAAAAAYAAAAAAAAAAAAAAAEAAAA");
//  command(L: 24 ) to send is:  "f90100000000000018000000000000000000000004000000"
//  received command(L: 24 ) is: "f90100000000000018000000000000000000000004000000"
  char sBuf[1024];
  EMC_TASK_SET_STATE ss;

  ss.state = EMC_TASK_STATE_ESTOP_RESET;
  memset(sBuf, 0, sizeof(sBuf));
  memcpy(sBuf, &ss, sizeof(ss));
  QByteArray sCmd;

  sCmd.setRawData((char*)&ss, sizeof(ss));
  QString sTest = sCmd.toBase64();

  qDebug() << "source command: " << sTest;
  QByteArray dCmd(dCmd.fromBase64(sTest.toLatin1()));
  char dBuf[1024];

  memset(dBuf, 0, sizeof(dBuf));
  memcpy(dBuf, dCmd.data(), dCmd.size());
//  EMC_TASK_SET_STATE* dSS = (EMC_TASK_SET_STATE*)dBuf;

  QCOMPARE(sBuf, dBuf);
  }


void UnitTests::testResponseParsing() {
  QSKIP("not now");
  QString rawMsg = QStringLiteral("HTTP/1.1 401 Unauthorized\r\n"
                                  "Connection: close\r\n"
                                  "Content-Type: text/html; charset=iso-8859-1\r\n"
                                  "Date: Sat, 22 Oct 2022 08:00:37 UTC\r\n"
                                  "Server: LCGateway\r\n"
                                  "nonce: 5682EBE6216BD72FEBBF7BAE391612EA\r\n"
                                  "Content-Length: 207\r\n"
                                  "\r\n");
  QString body = QStringLiteral("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n"
                                "<html><head>\n"
                                "<title>401 Unauthorized</title>\n"
                                "</head><body>\n"
                                "<h1>Unauthorized</h1>\n"
                                "<p>Unauthorized<br />\n"
                                "</p>\n"
                                "<hr>\n"
                                "<address>LCGateway</address>\n"
                                "</body></html>");
  HttpResponse r(HttpResponse().parse(rawMsg + body));
  QString test(r.toByteArray());

//  std::cout << "\nRESPONSE parsing:"   << std::endl << test.toStdString().c_str() << std::endl << std::endl;
  qDebug() << "length of body:" << body.size();
  QCOMPARE(test, rawMsg + body);
  }


void UnitTests::testAuthRequest() {
  QSKIP("not now");
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  auth  = new Authenticator();
  eLoop = new QEventLoop();
  sock  = new QTcpSocket();
  sock->setSocketOption(QTcpSocket::LowDelayOption, 1);

  auth->setup(QStringLiteral("/linuxcnc/command")
            , "me"
            , QString(QCryptographicHash::hash("sowas", QCryptographicHash::Md5).toHex()).toUpper());
  connect(sock, &QTcpSocket::connected,    this, &UnitTests::onConnected);
  connect(sock, &QTcpSocket::disconnected, this, &UnitTests::onDisconnect);

  nonce.clear();
  body.clear();
  body.append("do it!");
  sock->connectToHost(QHostAddress("10.9.9.19"), 62345);
  eLoop->exec();
  std::chrono::steady_clock::time_point end       = std::chrono::steady_clock::now();
  std::chrono::duration<double>         time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

  qDebug() << "authenticated reqest took:" << time_span.count() * 1000 << " milli-seconds";
  }


void UnitTests::testEStopCommand() {
  QSKIP("not now");
  std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
  auth  = new Authenticator();
  eLoop = new QEventLoop();
  sock  = new QTcpSocket();
  sock->setSocketOption(QTcpSocket::LowDelayOption, 1);

  auth->setup(QStringLiteral("/linuxcnc/command")
            , "me"
            , QString(QCryptographicHash::hash("sowas", QCryptographicHash::Md5).toHex()).toUpper());
  connect(sock, &QTcpSocket::connected,    this, &UnitTests::onConnected);
  connect(sock, &QTcpSocket::disconnected, this, &UnitTests::onDisconnect);
  EMC_TASK_SET_STATE ss;

//  ss.state = EMC_TASK_STATE_ESTOP_RESET;
  ss.state = EMC_TASK_STATE_ON;

  nonce.clear();
  body.clear();
  body.setRawData((char*)&ss, sizeof(ss));
  QString test(body.toHex());

  qDebug() << "command(L:" << body.size() << ") to send is: " << test;

  sock->connectToHost(QHostAddress("10.9.9.19"), 62345);
  eLoop->exec();
  std::chrono::steady_clock::time_point end       = std::chrono::steady_clock::now();
  std::chrono::duration<double>         time_span = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

  qDebug() << "authenticated reqest took:" << time_span.count() * 1000 << " milli-seconds";
  }


void UnitTests::onConnected() {
  qDebug() << "\tonConnected ...";
  QString uri = QStringLiteral("/linuxcnc/command");
  HttpRequest r(uri, QByteArray());

  if (nonce.isEmpty()) {
     connect(sock, &QTcpSocket::readyRead, this, &UnitTests::onDataRead);
     }
  else {
     QString realm = auth->calculateRealm(uri, nonce);

     r.setHeaderValue(QStringLiteral("Realm"), realm);
     r.setPayLoad(body.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));
     }
  QString test(r.toByteArray());

  qDebug() << "\t>>> request ===============";
  qDebug() << test;
  qDebug() << "\t<<< request ---------------";

  qint64 bytesWritten = sock->write(r.toByteArray());

  qDebug() << "wrote" << bytesWritten << "Bytes";
  }


void UnitTests::onDisconnect() {
  qDebug() << "\tonDisconnect ...";
  if (nonce.isEmpty())
     eLoop->exit();
  }


void UnitTests::onDataRead() {
  qDebug() << "\tonDataRead ...";
  char buffer[1024];
  int  bytesRead = sock->read(buffer, 1024);

  if (bytesRead < 1024) buffer[bytesRead] = '\0';
  else buffer[1023] = '\0';
  HttpResponse res(HttpResponse().parse(buffer));

  std::cout << buffer << std::endl;
  if (nonce.isEmpty() && res.responseCode() == ResponseCode::Unauthorized) {
     nonce = res.headerValue(QStringLiteral("nonce"));
     sock->close();
     // reconnect for authorized access
     sock->connectToHost(QHostAddress("10.9.9.19"), 62345);
     }
  else {
     if (res.responseCode() == ResponseCode::OK) {
        qDebug() << "response is success with" << res.payload();
        }
     QCOMPARE(res.responseCode(), ResponseCode::Accepted);
     nonce.clear();
     }
  }

QTEST_MAIN(UnitTests)
