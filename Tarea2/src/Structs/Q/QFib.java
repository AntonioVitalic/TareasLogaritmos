package structs.q;

import structs.graph.Node;
import java.util.ArrayList;
import java.util.List;

public class QFib {
    private List<FibNode> roots;
    private FibNode min;
    private int size;

    public QFib() { // aqui habia un int n como argumento del constructor, no es necesario implementarlo para construir la cola
        this.roots = new ArrayList<>();
        this.min = null;
        this.size = 0;
    }

    public void add(Node node, double priority) {
        FibNode newNode = new FibNode(node, priority);
        roots.add(newNode);
        if (min == null || priority < min.priority) {
            min = newNode;
        }
        size++;
    }

    public int extractMin() {
        if (min == null) return -1;
        int minNodeId = min.node.id;
        roots.remove(min);
        if (min.child != null) {
            for (FibNode child : min.child) {
                roots.add(child);
                child.parent = null;
            }
        }
        if (roots.isEmpty()) {
            min = null;
        } else {
            min = roots.get(0);
            for (FibNode root : roots) {
                if (root.priority < min.priority) {
                    min = root;
                }
            }
            consolidate();
        }
        size--;
        return minNodeId;
    }

    public void decreaseKey(Node node, double newPriority) {
        for (FibNode fibNode : roots) {
            if (fibNode.node.equals(node)) {
                if (newPriority < fibNode.priority) {
                    fibNode.priority = newPriority;
                    if (fibNode.parent != null && fibNode.priority < fibNode.parent.priority) {
                        cut(fibNode);
                    }
                    if (fibNode.priority < min.priority) {
                        min = fibNode;
                    }
                }
                break;
            }
        }
    }

    private void consolidate() {
        List<FibNode> newRoots = new ArrayList<>();
        for (FibNode root : roots) {
            newRoots.add(root);
            for (int i = 0; i < newRoots.size() - 1; i++) {
                if (root.degree == newRoots.get(i).degree) {
                    FibNode sameDegreeNode = newRoots.get(i);
                    if (sameDegreeNode.priority < root.priority) {
                        sameDegreeNode.child.add(root);
                        root.parent = sameDegreeNode;
                        root = sameDegreeNode;
                    } else {
                        root.child.add(sameDegreeNode);
                        sameDegreeNode.parent = root;
                    }
                    newRoots.remove(i);
                    i--;
                }
            }
        }
        roots = newRoots;
    }

    private void cut(FibNode fibNode) {
        if (fibNode.parent != null) {
            fibNode.parent.child.remove(fibNode);
            fibNode.parent = null;
            roots.add(fibNode);
        }
    }

    public boolean isEmpty() {
        return size == 0;
    }

    static class FibNode {
        Node node;
        double priority;
        FibNode parent;
        List<FibNode> child;
        int degree;

        FibNode(Node node, double priority) {
            this.node = node;
            this.priority = priority;
            this.child = new ArrayList<>();
            this.degree = 0;
        }
    }
}
