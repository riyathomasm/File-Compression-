import networkx as nx
import matplotlib.pyplot as plt

def build_huffman_tree_from_file(filename):
    tree = nx.DiGraph()
    with open(filename, "r") as file:
        for line in file:
            parts = line.strip().split()
            if len(parts) == 2:
                char, freq = parts
                tree.add_node(char, weight=int(freq))
    
    return tree

def visualize_huffman_tree(tree):
    pos = nx.spring_layout(tree)
    labels = {node: f"{node}\n({tree.nodes[node]['weight']})" for node in tree.nodes}
    plt.figure(figsize=(8, 6))
    nx.draw(tree, pos, with_labels=True, labels=labels, node_color="lightblue", edge_color="gray", node_size=2000, font_size=12, font_weight="bold")
    plt.title("Huffman Tree Visualization")
    plt.show()

if __name__ == "__main__":
    tree = build_huffman_tree_from_file("tree_output.txt")
    visualize_huffman_tree(tree)
