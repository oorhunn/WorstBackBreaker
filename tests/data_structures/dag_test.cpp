//
// Created by Anıl Orhun Demiroğlu on 2.08.2024.
//
#include <gtest/gtest.h>
#include "dag.hpp"

using namespace ml::neat;

TEST(DAGTest, CreateNode) {
DAG dag;
for (int i = 0; i < 5; ++i) {
dag.create_node();
}
EXPECT_EQ(dag.m_nodes.size(), 5);
}

TEST(DAGTest, CreateValidConnection) {
DAG dag;
for (int i = 0; i < 5; ++i) {
dag.create_node();
}
EXPECT_TRUE(dag.create_connection(0, 1));
EXPECT_EQ(dag.m_nodes[0].get_out_connection_count(), 1);
EXPECT_EQ(dag.m_nodes[1].incoming, 1);
}

TEST(DAGTest, CreateInvalidConnectionSelfLoop) {
DAG dag;
for (int i = 0; i < 5; ++i) {
dag.create_node();
}
EXPECT_FALSE(dag.create_connection(0, 0));
}

TEST(DAGTest, CreateInvalidConnectionCycle) {
DAG dag;
for (int i = 0; i < 5; ++i) {
dag.create_node();
}
EXPECT_TRUE(dag.create_connection(0, 1));
EXPECT_TRUE(dag.create_connection(1, 2));
EXPECT_FALSE(dag.create_connection(2, 0));
}

TEST(DAGTest, IsAncestor) {
DAG dag;
for (int i = 0; i < 5; ++i) {
dag.create_node();
}
dag.create_connection(0, 1);
dag.create_connection(1, 2);
dag.create_connection(2, 3);

EXPECT_TRUE(dag.is_ancestor(0, 3));
EXPECT_FALSE(dag.is_ancestor(3, 0));
}

TEST(DAGTest, IsParent) {
DAG dag;
for (int i = 0; i < 5; ++i) {
dag.create_node();
}
dag.create_connection(0, 1);
EXPECT_TRUE(dag.is_parent(0, 1));
EXPECT_FALSE(dag.is_parent(1, 0));
}

TEST(DAGTest, ComputeDepth) {
DAG dag;
for (int i = 0; i < 5; ++i) {
dag.create_node();
}
dag.create_connection(0, 1);
dag.create_connection(1, 2);
dag.create_connection(2, 3);
dag.create_connection(0, 4);

dag.compute_depth();

EXPECT_EQ(dag.m_nodes[0].depth, 0);
EXPECT_EQ(dag.m_nodes[1].depth, 1);
EXPECT_EQ(dag.m_nodes[2].depth, 2);
EXPECT_EQ(dag.m_nodes[3].depth, 3);
EXPECT_EQ(dag.m_nodes[4].depth, 1);
}

TEST(DAGTest, GetOrder) {
DAG dag;
for (int i = 0; i < 5; ++i) {
dag.create_node();
}
dag.create_connection(0, 1);
dag.create_connection(1, 2);
dag.create_connection(2, 3);
dag.create_connection(0, 4);

dag.compute_depth();
std::vector<uint32_t> order = dag.get_order();

EXPECT_EQ(order[0], 0);
EXPECT_EQ(order[1], 1);
EXPECT_EQ(order[2], 4);
EXPECT_EQ(order[3], 2);
EXPECT_EQ(order[4], 3);
}

TEST(DAGTest, RemoveConnection) {
DAG dag;
for (int i = 0; i < 5; ++i) {
dag.create_node();
}
dag.create_connection(0, 1);
dag.create_connection(1, 2);

dag.remove_connection(1, 2);

EXPECT_EQ(dag.m_nodes[1].get_out_connection_count(), 0);
EXPECT_EQ(dag.m_nodes[2].incoming, 0);
}
