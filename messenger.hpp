#ifndef MESSENGER_HPP
#define MESSENGER_HPP
/**********************************************************************/
/*   <copyright                                                       */
/*   notice="oco-source"                                              */
/*   pids="5755-P60"                                                  */
/*   years="2013"                                                     */
/*   crc="2536674324" >                                               */
/*   IBM Confidential                                                 */
/*                                                                    */
/*   OCO Source Materials                                             */
/*                                                                    */
/*   5755-P60                                                         */
/*                                                                    */
/*   (C) Copyright IBM Corp. 2013                                     */
/*                                                                    */
/*   The source code for the program is not published                 */
/*   or otherwise divested of its trade secrets,                      */
/*   irrespective of what has been deposited with the                 */
/*   U.S. Copyright Office.                                           */
/*   </copyright>                                                     */
/*                                                                    */
/**********************************************************************/
/* Following text will be included in the Service Reference Manual.   */
/* Ensure that the content is correct and up-to-date.                 */
/* All updates must be made in mixed case.                            */
/*                                                                    */
/* The functions in this file provide the wrapper functions around    */
/* the Apache Qpid Proton C Messenger API for use by Node.js          */
/**********************************************************************/
/* End of text to be included in SRM                                  */
/**********************************************************************/

#include <string>

#include <node.h>

#include <proton/message.h>
#include <proton/messenger.h>
#include <proton/condition.h>
#include <proton/terminus.h>
#include <proton/link.h>
#include <proton/transport.h>
#include <proton/connection.h>

class ProtonMessenger : public node::ObjectWrap
{
 public:
  static v8::Persistent<v8::FunctionTemplate> constructor;
  static void Init(v8::Handle<v8::Object> target);
  static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);
  ProtonMessenger(std::string name, std::string username, std::string password);
  ~ProtonMessenger();

 protected:
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> Put(const v8::Arguments& args);
  static v8::Handle<v8::Value> Send(const v8::Arguments& args);
  static v8::Handle<v8::Value> Stop(const v8::Arguments& args);
  static v8::Handle<v8::Value> Stopped(v8::Local<v8::String> property,
                                       const v8::AccessorInfo& info);
  static v8::Handle<v8::Value> Connect(const v8::Arguments& args);
  static v8::Handle<v8::Value> Subscribe(const v8::Arguments& args);
  static v8::Handle<v8::Value> Receive(const v8::Arguments& args);
  static v8::Handle<v8::Value> Status(const v8::Arguments& args);
  static v8::Handle<v8::Value> Accept(const v8::Arguments& args);
  static v8::Handle<v8::Value> Settle(const v8::Arguments& args);
  static v8::Handle<v8::Value> HasOutgoing(v8::Local<v8::String> property,
                                           const v8::AccessorInfo& info);
  static v8::Handle<v8::Value> GetLastErrorText(const v8::Arguments& args);
  static v8::Handle<v8::Value> GetRemoteIdleTimeout(const v8::Arguments& args);
  static v8::Handle<v8::Value> Work(const v8::Arguments& args);
  static v8::Handle<v8::Value> Flow(const v8::Arguments& args);
  static void Tracer(pn_transport_t* transport, const char* message);

  /**
   * Name for the messenger. Initially this will be set to the value passed to
   * the constructor.
   * When the proton messenger is constructed this may be modified to the name
   * passed back from
   * the pn_messenger_name function. In general it will not change.
   */
  std::string name;

  /**
   * Username, non-blank implies SASL authentication required.
   */
  std::string username;

  /**
   * Password for a specified username, when SASL authentication required.
   */
  std::string password;

  /**
   * Points to the underlying proton messenger. This will be set when Connect is
   * called and unset when stop is called.
   */
  pn_messenger_t* messenger;

  /**
   * The error message text for the last connect error. This is used by the
   * GetLastErrorText method when we do not have
   * a connection, as the error text comes from the proton messenger, which we
   * free up on a connection error.
   */
  std::string lastConnectErrorText;
};

#endif /* MESSENGER_HPP */
