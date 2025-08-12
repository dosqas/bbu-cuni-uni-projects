#pragma once
#include <iostream>
#include <exception>
#include <string>


class UIException : public std::exception
{
	private:
		std::string m_message;

	public:
		UIException(const std::string& message) : m_message(message) {}
		virtual const char* what() const noexcept override { return m_message.c_str(); }
};

class ServException : public std::exception
{
private:
	std::string m_message;
public:
	ServException(const std::string& message) : m_message(message) {}
	virtual const char* what() const noexcept override { return m_message.c_str(); }
};

class RepoException : public std::exception
{
	private:
		std::string m_message;
	public:
		RepoException(const std::string& message) : m_message(message) {}
		virtual const char* what() const noexcept override { return m_message.c_str(); }
};