#include "graphnode.h"
#include "graphedge.h"
#include <memory>

GraphNode::GraphNode(int id) { _id = id; }

GraphNode::~GraphNode() {
  //// STUDENT CODE
  ////
  // task 0
  // delete _chatBot;

  ////
  //// EOF STUDENT CODE
}

void GraphNode::AddToken(std::string token) { _answers.push_back(token); }

void GraphNode::AddEdgeToParentNode(GraphEdge *edge) {
  _parentEdges.push_back(edge);
}

// task 4
// void GraphNode::AddEdgeToChildNode(GraphEdge *edge)
void GraphNode::AddEdgeToChildNode(std::unique_ptr<GraphEdge> edge) {
  // _childEdges.push_back(edge);
  _childEdges.push_back(std::move(edge));
}

//// STUDENT CODE
////

// task 5
// void GraphNode::MoveChatbotHere(ChatBot *chatbot) {
//   _chatBot = chatbot;
//   _chatBot->SetCurrentNode(this);
// }

void GraphNode::MoveChatbotHere(ChatBot chatbot) {
  _chatBot = std::move(chatbot);
  _chatBot.SetCurrentNode(this);
}

// void GraphNode::MoveChatbotToNewNode(GraphNode *newNode) {
//   newNode->MoveChatbotHere(_chatBot);
//   _chatBot = nullptr; // invalidate pointer at source
// }
void GraphNode::MoveChatbotToNewNode(GraphNode *newNode) {
  newNode->MoveChatbotHere(std::move(_chatBot));
}

////
//// EOF STUDENT CODE

GraphEdge(*GraphNode::GetChildEdgeAtIndex(int index)) {
  //// STUDENT CODE
  ////
  // task 4
  // return _childEdges[index];
  return (_childEdges[index]).get();

  ////
  //// EOF STUDENT CODE
}