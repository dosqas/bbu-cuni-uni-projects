package cz.cuni.mff.sopteles.programs;

import cz.cuni.mff.sopteles.utils.BinarySearchTree;
import cz.cuni.mff.sopteles.utils.Iterator;

public class Main {
    public static void main(String[] args) {
        BinarySearchTree bst = new BinarySearchTree();

        for (String arg : args) {
            try {
                int number = Integer.parseInt(arg);
                bst.add(number);
            }
            catch (Exception e) {
                System.out.println("INPUT ERROR");
                System.exit(0);
            }
        }

        Iterator iter = bst.getIterator();

        while (iter.hasNext()) {
            System.out.println(iter.getValue());
            iter.next();
        }
    }
}