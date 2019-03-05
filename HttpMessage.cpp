#include "HttpMessage.h"
#include <iostream>

ZiApi::HttpMessage::HttpMessage()
	: header(std::make_shared<HttpField>()), body(std::make_shared<HttpField>())
{
}
ZiApi::HttpMessage::HttpMessage(const std::shared_ptr< ZiApi::HttpField> &header, const  std::shared_ptr< ZiApi::HttpField> &body)
	: header(header), body(body)
{
}

ZiApi::HttpMessage::HttpMessage(const std::string &header, const std::string &body)
{
	this->header = std::make_shared<HttpField>(header);
	this->body = std::make_shared<HttpField>(body);
}

ZiApi::HttpMessage::~HttpMessage()
{
}


std::shared_ptr< ZiApi::HttpField>	ZiApi::HttpMessage::getHeader()const
{
	return (header);
}

std::shared_ptr< ZiApi::HttpField>	ZiApi::HttpMessage::getBody()const
{
	return (body);
}

void								ZiApi::HttpMessage::setHeader(const std::string &header)
{
	this->header->setField(header);
}

void								ZiApi::HttpMessage::setBody(const std::string &body)
{
	this->body->setField(body);
}

void								ZiApi::HttpMessage::setMessage(const std::string &message)
{
	std::string						delimiters("\r\n\r\n");
	auto							end = message.find(delimiters);


	this->setHeader(message.substr(0, end));
	if (end + delimiters.length() < message.length() && end != std::string::npos) {
		this->setBody(message.substr(end + delimiters.length()));
	}
}

std::string							ZiApi::HttpMessage::getMessage()const
{
	return (header->getStringField() + body->getStringField());
}