from __future__ import print_function
import  collections
from itertools import imap
from collections import defaultdict, namedtuple
import itertools

def remove_values_from_list(the_list, val):
   return [value for value in the_list if value != val]

#  load from file functions
def load_products(filename_products, products):
    f = open(filename_products, 'r')
    for line in f:
        lineList = line.split()
        if lineList:
            name = ""
            for index in range(len(lineList)-1):
                name += lineList[index]
            products.append(name)
    f.close()

def load_transactions(filename_transactions, transactions):
    f = open(filename_transactions, 'r')
    for line in f:
        lineList = line.split()
        transaction = []
        for index in range(len(lineList)):
            if index != 0: # ignore first element

                if ( int(lineList[index][0]) != 0):
                    transaction.append((index - 1))
        transactions.append(transaction)
    f.close()

#  EXPLAIN THIS
def bubble_sort(items,freqList):
    for i in range(len(items)):
        for j in range(len(items)-1-i):
            for element in freqList:
                if items[j] == element[0]:
                    freq0 = element[1]
                if items[j+1] == element[0]:
                    freq1 = element[1]
            if freq0 < freq1:
                items[j], items[j+1] = items[j+1], items[j]

#  tree functions EXPLAIN THIS
class FPTree(object):

    Route = namedtuple('Route', 'head tail')

    def __init__(self):
        # The root node of the tree.
        self._root = FPNode(self, None, None)

        # dictionary mapping items to the head and tail of a path of
        # neighbors that will hit every node containing that item
        self._routes = {}

    @property
    def root(self):
        # root node of tree
        return self._root

    def add(self, transaction):
       #  add transaction to tree
        point = self._root

        for item in transaction:
            next_point = point.search(item)
            if next_point:
                # There is already a node in this tree for the current
                # transaction item, then use it again
                next_point.increment()
            else:
                # Create a new point and add it as a child of the point currently
                # being looking at
                next_point = FPNode(self, item)
                point.add(next_point)

                # Update the route of nodes that contain this item to include
                # the new node
                self._update_route(next_point)

            point = next_point

    def _update_route(self, point):
        """Add the given node to the route through all nodes for its item """
        assert self is point.tree

        try:
            route = self._routes[point.item]
            route[1].neighbor = point # route[1] is the tail
            self._routes[point.item] = self.Route(route[0], point)
        except KeyError:
            # First node for this item; start a new route.
            self._routes[point.item] = self.Route(point, point)

    def items(self):
        """
        Generate one 2-tuples for each item represented in the tree. The first
        element of the tuple is the item itself, and the second element is a
        generator that will yield the nodes in the tree that belong to the item.
        """
        for item in self._routes.iterkeys():
            yield (item, self.nodes(item))

    def nodes(self, item):
        """
        Generate the sequence of nodes that contain the given item
        """

        try:
            node = self._routes[item][0]
        except KeyError:
            return

        while node:
            yield node
            node = node.neighbor

    def prefix_paths(self, item):
        """Generate  prefix paths that end with the given item"""

        def collect_path(node):
            path = []
            while node and not node.root:
                path.append(node)
                node = node.parent
            path.reverse()
            return path

        return (collect_path(node) for node in self.nodes(item))

    def inspect(self):
        print ('Tree:')
        self.root.inspect(1)

        print
        print ('Routes:')
        for item, nodes in self.items():
            print ('  %r' % item)
            for node in nodes:
                print ('    %r' % node)

def cond_tree_from_paths(paths):
    """Build a conditional FP-tree from the given prefix paths."""
    tree = FPTree()
    condition_item = None
    items = set()

    """"
    # Import  nodes in the paths into the new tree... Nur die Zahlen von leaf nodes sind wichtig...
     the remaining counts will be reconstructed from the eaf counts
    """
    for path in paths:
        if condition_item is None:
            condition_item = path[-1].item

        point = tree.root
        for node in path:
            next_point = point.search(node.item)
            if not next_point:
                # Add a new node to the tree.
                items.add(node.item)
                count = node.count if node.item == condition_item else 0
                next_point = FPNode(tree, node.item, count)
                point.add(next_point)
                tree._update_route(next_point)
            point = next_point

    assert condition_item is not None

    # Calc the counts of the non-leaf nodes
    for path in tree.prefix_paths(condition_item):
        count = path[-1].count
        for node in reversed(path[:-1]):
            node._count += count

    return tree

class FPNode(object):

    def __init__(self, tree, item, count=1):
        self._tree = tree
        self._item = item
        self._count = count
        self._parent = None
        self._children = {}
        self._neighbor = None

    def add(self, child):
        # add node as childe of this node

        self._children[child.item] = child
        child.parent = self

    def search(self, item):

        # if this node contains a child node for the given item, that node is returned... else `None` is returned

        try:
            return self._children[item]
        except KeyError:
            return None

    def __contains__(self, item):
        return item in self._children

    @property
    def tree(self):
        return self._tree

    @property
    def item(self):
        return self._item

    @property
    def count(self):
        return self._count

    def increment(self):
        self._count += 1

    @property
    def root(self):
        return self._item is None and self._count is None

    @property
    def leaf(self):
        return len(self._children) == 0

    @property
    def parent(self):
        return self._parent

    @parent.setter
    def parent(self, value):
        self._parent = value

    @property
    def neighbor(self):
        """
        The node's neighbor... the one with the same value that is "to the right"
        of it in the tree
        """
        return self._neighbor

    @neighbor.setter
    def neighbor(self, value):
        self._neighbor = value

    @property
    def children(self):
        return tuple(self._children.itervalues())

    # def inspect(self, depth=0):
    #     print ('  ' * depth) + repr(self)
    #     for child in self.children:
    #         child.inspect(depth + 1)

    # def __repr__(self):
    #     if self.root:
    #         return "<%s (root)>" % type(self).__name__
    #     return "<%s %r (%r)>" % (type(self).__name__, self.item, self.count)


# EXPLAIN THIS:
#  removes unfrequent products from transactions, then, for each transaction, order its freqeuent items according to the order in freqList.
def frequentizeAndOrder(transactions, freq_Thresh):
    coll = collections.Counter(x for xs in transactions for x in set(xs))
    total = float(sum(coll.values()))
    freqList_prelim = [x for x in coll.most_common()]
    freqList = []
    list_element = []
    transactions_adjusted = []
    toBeRemoved = []

    # print(freqList)

    for i in range(len(freqList_prelim)):
        list_element = []

        if (freqList_prelim[i][1] / total > freq_Thresh):
            list_element.append(freqList_prelim[i][0])
            list_element.append(freqList_prelim[i][1])
            freqList.append(list_element)
        else:
            #  go through entire list, remove all occurances of this value that is below support_thresh
            toBeRemoved.append(freqList_prelim[i][0])

        #  construct new list of lists with only frequent values
    for transaction in transactions:
        tempTransaction = []
        for element in transaction:
            valOK = True
            for item in toBeRemoved:
                if item == element:
                    valOK = False
            if valOK:
                tempTransaction.append(element)

        transactions_adjusted.append(tempTransaction)

#  Now, for each transaction, order its freqeuent items according to the order in freqList.

    for transaction in transactions_adjusted:
        bubble_sort(transaction,freqList)

    print("went from ", len(freqList_prelim), "products to ", len(freqList))

    return transactions_adjusted

def combinations(stuff):
    myList = []
    for L in range(1, len(stuff)):
        for subset in itertools.combinations(stuff, L):
            listy = []
            for element in subset:
                listy.append(element)
            myList.append(listy)
    return myList


def generateRules(transactions,products,minimum_support,minimum_confidence):
    master = FPTree()

    for transaction in transactions:
        master.add(transaction)

    total = float(len(transactions))

    print ("total # of transactions is: ", total)

    supportDict = {}

    def createSupportDict(tree, suffix ):
        for item, nodes in tree.items():
            supportCount = sum(n.count for n in nodes)
            support = supportCount / total
            found_set = [item] + suffix
            yield (found_set, support)

            if support >= minimum_support and item not in suffix:
                cond_tree = cond_tree_from_paths(tree.prefix_paths(item))
                for s in createSupportDict(cond_tree, found_set):
                    yield s

    #  make support dict for all sets
    for itemsetWithSupport in createSupportDict(master, []):
        supportDict[tuple(itemsetWithSupport[0])] = itemsetWithSupport[1]

    def find_with_suffix(tree, suffix):

        for item, nodes in tree.items():
            supportCount = sum(n.count for n in nodes)
            support = supportCount / total

            if support >= minimum_support and item not in suffix:

                found_set = [item] + suffix
                set_parent = [item]

                yield (found_set, support)

                # build a conditional tree and recursively search for frequent
                # itemsets within it
                cond_tree = cond_tree_from_paths(tree.prefix_paths(item))
                for s in find_with_suffix(cond_tree, found_set):
                    yield s

    # search for frequent itemsets, yield found results
    freqItemSet = []
    myList = []

    for itemset in find_with_suffix(master, []):
        freqItemSet.append(itemset)

    for x in freqItemSet:
        subList = []
        # print(x)
        subList = x
        if len(subList[0]) > 1: # EXPLAIN THIS
            myList.append(subList)

    # print("mylist0: ", myList[0])
    # myList = sorted(myList, key=lambda x: x[1], reverse=True)

    largestK = 0;

    for item in myList:
        count = len(item[0])
        # for product in item[0]:
        #     print(products[product], end = " ")
        # print()

        if count > largestK:
            largestK = count

        # print(item[1], "K= ", count)

    #  find largest K
    countOnK = [0] * largestK # create list to count frequency of itemsets by K
    for item in myList:
        countOnK[ len(item[0]) - 1] = (countOnK[ len(item[0]) - 1] + 1)

    # Number of frequent item sets (re port foreach K)
    for kVal in range(len(countOnK) ):
        if not (kVal == 0):
            print("Number of frequent item sets with K =", (kVal + 1), "is", countOnK[kVal])
    # print("Number of frequent item sets with K =", largestK, "is", countOnK[largestK])

    print("Largest K is:", largestK)

    rules = []

    for item in myList:
        itemSet = item[0]
        listOfSubsets = combinations(itemSet)
        # print(listOfSubsets)
        # print(itemSet)
        for subset in listOfSubsets:         # for each subest c of i:
            # print(subset, ",", end = " ")

            itemSetMinusSubSet = []

            for i in itemSet[:]: # remove common elements from itemSet that are in subSet
                if i not in subset:
                    itemSetMinusSubSet.append(i)

            confidence = supportDict[tuple(itemSet)] / supportDict[tuple(itemSetMinusSubSet)]
            if (confidence >= minimum_confidence):

                rule = [ itemSetMinusSubSet, subset, supportDict[tuple(subset)], confidence  ]
                rules.append(rule)
                # print(rule)

                # print(itemSetMinusSubSet, "-->", subset, "confidence =", confidence, "and support =", supportDict[tuple(subset)] ) # print
    rules = sorted(rules, key=lambda x: x[2], reverse=True)

    print("Number of rules generated is:", (len(rules)))

    for rule in rules:
        for item in rule[0]:
            print(products[item], end = " ")

        print("-->", end = " ")
        for item in rule[1]:
            print(products[item], end = " ")
        print("support = ", rule[2], "and confidence =", rule[3])

#  main
def main():

    #  DEFINE TRANSACTION AND PRODUTS FILES HERE
    transactions_file = "/Users/Konrad/Desktop/ar_test_data/small_basket.dat"
    products_file = "/Users/Konrad/Desktop/ar_test_data/products"

      #  SPECIFY MIN SUPPORT AND CONFIDENCE HERE
    support_min = .4 # to remove item sets based on support
    confidence_min = 0.6 # to remove item sets based on confidence

    freq_minThresh = .002 # to remove unfrequent products

    products = list()
    transactions_prelim = []
    load_products(products_file, products)
    load_transactions(transactions_file , transactions_prelim)

    transactions = frequentizeAndOrder(transactions_prelim, freq_minThresh)

    generateRules(transactions, products, support_min,confidence_min)


if __name__ == "__main__":
    main()

