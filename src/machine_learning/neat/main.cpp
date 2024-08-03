//
// Created by Anıl Orhun Demiroğlu on 21.07.2024.
//

#include <SFML/Graphics.hpp>
#include "DAG.hpp"

using namespace ml::neat;

// Function to visualize the DAG
void visualizeDAG(DAG &dag) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "DAG Visualization");

    std::vector<sf::CircleShape> nodeShapes;
    std::vector<sf::VertexArray> connections;

    float radius = 20.f;
    float offsetX = 100.f;
    float offsetY = 100.f;

    for (size_t i = 0; i < dag.m_nodes.size(); ++i) {
        sf::CircleShape nodeShape(radius);
        nodeShape.setFillColor(sf::Color::Green);
        nodeShape.setOrigin(radius, radius);

        float x = (i % 5) * offsetX + offsetX;
        float y = (i / 5) * offsetY + offsetY;

        nodeShape.setPosition(x, y);
        nodeShapes.push_back(nodeShape);

        for (auto outNode : dag.m_nodes[i].out) {
            sf::VertexArray line(sf::Lines, 2);
            line[0].position = sf::Vector2f(x, y);
            line[1].position = sf::Vector2f((outNode % 5) * offsetX + offsetX, (outNode / 5) * offsetY + offsetY);
            line[0].color = sf::Color::White;
            line[1].color = sf::Color::White;
            connections.push_back(line);
        }
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        for (const auto &line : connections) {
            window.draw(line);
        }

        for (const auto &nodeShape : nodeShapes) {
            window.draw(nodeShape);
        }

        window.display();
    }
}

int main() {
    DAG dag;
    // Example of adding nodes and connections
    for (int i = 0; i < 10; ++i) {
        dag.create_node();
    }
    dag.create_connection(0, 1);
    dag.create_connection(1, 2);
    dag.create_connection(2, 3);
    dag.create_connection(3, 4);
    dag.create_connection(4, 5);
    dag.create_connection(5, 6);
    dag.create_connection(6, 7);
    dag.create_connection(7, 8);
    dag.create_connection(8, 9);

    // Compute depth to properly visualize the DAG
    dag.compute_depth();

    visualizeDAG(dag);

    return 0;
}