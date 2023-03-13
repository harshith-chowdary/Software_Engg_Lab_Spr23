class _DoubleLinkedBase:
	""" A base class providing a doubly linked list representation."""

	class _Node:
		""" Lightweight, nonpublic class for storing a doubly linked node"""
		__slots__ = '_element', '_prev', '_next' # streamline memory

		def __init__(self, element, prev, next): # initialize node's fields
			self._element = element
			self._prev = prev
			self._next = next

	def __init__(self):
		"""Create an empty list"""
		self._header = self._Node(None, None, None)
		self._trailer = self._Node(None, None, None)
		self._header._next = self._trailer
		self._trailer._prev = self._header
		self._size = 0 # number of elements

	def __len__(self):
		"""Return the number of elements in the list"""
		# return self._size
	# or bottom method
		len = 0
		node = self._header
		while(node._next != self._trailer):
			len += 1
			node = node._next
		return len
		# ===== Start writing your code here =====
		pass # Remove this statement once you write your code
		# ===== End writing your code here =====

	def is_empty(self):
		"""Return true if list is empty"""
		if(self._header._next == self._trailer):
			return True
		return False
		# ===== Start writing your code here =====
		pass # Remove this statement once you write your code
		# ===== End writing your code here =====

	def _insert_between(self, e, predecessor, successor):
		"""Add element e between two existing nodes and return new node"""
		newest = self._Node(e, predecessor, successor)
		predecessor._next = newest
		successor._prev = newest
		self._size += 1
		return newest
		# ===== Start writing your code here =====
		pass # Remove this statement once you write your code
		# ===== End writing your code here =====

	def _delete_node(self, node):
		"""Delete nonsentinel node from the list and return its elements"""
		temp = self._header._next
		while(temp != node):
			temp = temp._next
		temp._prev._next = temp._next
		temp._next._prev = temp._prev
		self._size -= 1
		return temp._element
		# ===== Start writing your code here =====
		pass # Remove this statement once you write your code
		# ===== End writing your code here =====

n = _DoubleLinkedBase()
n5 = n._insert_between(5, n._header, n._trailer)
print(n5._element)

print(n.__len__())

n6 = n._insert_between(6, n5, n._trailer)
print(n6._element)

n4 = n._insert_between(4, n._header ,n5)
print(n4._element)

print(n.__len__())

n9 = n._insert_between(9, n5, n6)
print(n9._element)

n9 = n._insert_between(9, n5, n6)

ele = n._delete_node(n9)

print(ele)

print(n.is_empty())

# uncomment below lines for testing

# 	def _display_all(self):
# 		if(self.is_empty()):
# 			print("Null")
# 			return
# 		node = self._header
# 		while(node._next != self._trailer):
# 			node = node._next
# 			print(node._element, end=" ")
# 		print()

# dll = _DoubleLinkedBase()

# print(dll.is_empty())

# # print(dll.__len__())

# dll._display_all()

# node1 = dll._insert_between(1, dll._header, dll._trailer)

# # print(dll.__len__())

# dll._display_all()

# node3 = dll._insert_between(3, node1, dll._trailer)

# # print(dll.__len__())

# dll._display_all()

# node2 = dll._insert_between(2, node1, node3)

# # print(dll.__len__())

# dll._display_all()

# dll._delete_node(node2)

# # print(dll.__len__())

# dll._display_all()

# dll._delete_node(node1)

# dll._display_all()

# dll._delete_node(node3)

# print(dll.is_empty())