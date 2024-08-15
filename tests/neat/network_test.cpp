//
// Created by Anıl Orhun Demiroğlu on 6.08.2024.
//

#include <gtest/gtest.h>
#include "network.hpp"

using namespace ml::neat;

TEST(NetworkTest, Initialization) {
    Network network;
    Network::Info info(3, 2);
    network.initialize(info, 4);

    EXPECT_EQ(network.m_info.m_inputs, 3);
    EXPECT_EQ(network.m_info.m_outputs, 2);
    EXPECT_EQ(network.m_info.m_hidden, 0);
    EXPECT_EQ(network.m_connection_count, 4);
    EXPECT_EQ(network.m_slots.size(), info.get_node_count() + 4);
    EXPECT_EQ(network.m_output.size(), 2);
}

TEST(NetworkTest, SetNode) {
    Network network;
    Network::Info info(2, 1);
    network.initialize(info, 3);

    network.set_node(0, Activation::Sigmoid, 1.0, 2);
    network.set_node(1, Activation::Tanh, -0.5, 1);

    auto &node0 = network.get_node(0);
    auto &node1 = network.get_node(1);

    EXPECT_EQ(node0.m_bias, 1.0);
    EXPECT_EQ(node0.m_connection_count, 2);
    EXPECT_EQ(node1.m_bias, -0.5);
    EXPECT_EQ(node1.m_connection_count, 1);
}

TEST(NetworkTest, SetConnection) {
    Network network;
    Network::Info info(2, 1);
    network.initialize(info, 3);

    network.set_connection(0, 2, 0.5);
    network.set_connection(1, 2, -1.0);
    network.set_connection(2, 1, 0.75);

    auto &conn0 = network.get_connection(0);
    auto &conn1 = network.get_connection(1);
    auto &conn2 = network.get_connection(2);

    EXPECT_EQ(conn0.m_to, 2);
    EXPECT_EQ(conn0.m_weight, 0.5);
    EXPECT_EQ(conn1.m_to, 2);
    EXPECT_EQ(conn1.m_weight, -1.0);
    EXPECT_EQ(conn2.m_to, 1);
    EXPECT_EQ(conn2.m_weight, 0.75);
}

TEST(NetworkTest, Execute) {
    Network network;
    Network::Info info(2, 1);
    network.initialize(info, 3);

    network.set_node(0, Activation::None, 0.0, 1);
    network.set_node(1, Activation::None, 0.0, 1);
    network.set_node(2, Activation::Sigmoid, 0.0, 0);

    network.set_connection(0, 2, 0.5);
    network.set_connection(1, 2, 0.5);

    std::vector<std::double_t> input = {1.0, 1.0};
    bool result = network.execute(input);

    EXPECT_TRUE(result);
    EXPECT_NEAR(network.get_result()[0], 0.622459, 0.000001);
}
