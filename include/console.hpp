#pragma once

class Parser;
class EventQueue;

/**
  Responsible for receiving user input and processing it
  */

class Console{
	public:
		Console(EventQueue * queue);
		~Console();

		// runs an infinite input loop
		void run();
	private:	
		Parser * parser;
};
