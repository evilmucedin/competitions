#pragma once

#include "action.h"
#include "info.h"
#include "insert.h"
#include "node.h"
#include "nodes_manager.h"
#include "rotate.h"
#include "sibling.h"
#include "swap.h"

template<class TInfo>
class RBTInfo : public TInfo
{
public:
	bool is_black = false;
};

template <
	class TTData,
	class TTInfo = BSTInfoSize,
	class TTAction = BSTActionNone,
	class TTKey = int64_t>
class RedBlackTree : public BSTNodesManager<BSTNode<TTData, RBTInfo<TTInfo>, TTAction, true, true, false, TTKey>,
											RedBlackTree<TTData, TTInfo, TTAction, TTKey>>
{
public:
	static const bool use_key = true;
	static const bool use_parent = true;
	static const bool use_height = false;

	using TData = TTData;
	using TInfo = RBTInfo<TTInfo>;
	using TAction = TTAction;
	using TKey = TTKey;
	using TNode = BSTNode<TData, TInfo, TAction, use_key, use_parent, use_height, TKey>;
	using TSelf = RedBlackTree<TData, TTInfo, TAction, TKey>;
	using TNodesManager = BSTNodesManager<TNode, TSelf>;

public:
	RedBlackTree(unsigned max_nodes) : TNodesManager(max_nodes) {}

	static TNode* BuildTree(const vector<TNode*>& nodes)
	{
		TNode* root = 0;
		for (TNode* node : nodes)
			root = Insert(root, node);
		return root;
	}
	
	static TNode* Insert(TNode* root, TNode* node)
	{
		BSTInsert<TNode>(root, node);
		node->info.is_black = false;
		for (;;)
		{
			TNode* parent = node->p;
			if (!parent) { node->info.is_black = true; return node; }
			if (parent->info.is_black) return root;
			TNode* gparent = parent->p;
			TNode* uncle = BSTSibling(parent, gparent);
			if (!uncle || uncle->info.is_black)
			{
				bool rotate_required = ((gparent->l == parent) != (parent->l == node));
				if (rotate_required)
				{
					BSTRotate<TNode, false>(node, parent);
					parent = node;
				}
				BSTRotate<TNode, true>(parent, gparent);
				gparent->info.is_black = false;
				parent->info.is_black = true;
				return parent->p ? root : parent;
			}
			parent->info.is_black = true;
			uncle->info.is_black = true;
			gparent->info.is_black = false;
			node = gparent;
		}
		assert(false);
		return 0;
	}

	static TNode* Remove(TNode* node)
	{
		assert(node);
		ApplyActionRootToNode(node);

		// Optional swap
		if (node->l && node->r)
		{
			TNode* temp = node->l;
			for (temp->ApplyAction(); temp->r; )
			{
				temp = temp->r;
				temp->ApplyAction();
			}
			BSTSwap<TNode, false>(node, temp);
			swap(node->info.is_black, temp->info.is_black);
		}

		// Drop node from tree
		TNode* child = node->l ? node->l : node->r;
		TNode* parent = node->p;
		if (parent)
		{
			if (parent->l == node)
				parent->l = child;
			else
				parent->r = child;
		}
		if (child) child->p = parent;
		node->ResetLinks();
		UpdateInfoNodeToRoot(parent);

		// Fix colors
		if (!node->info.is_black) return (parent ? Root(parent) : child);
		for (;;)
		{
			if (child && !child->info.is_black)
			{
				child->info.is_black = true;
				return Root(child);
			}
			if (!parent) return child;
			TNode* sibling = BSTSibling(child, parent);
			assert(sibling);
			if (!sibling->info.is_black)
			{
				assert(parent->info.is_black);
				BSTRotate<TNode, true>(sibling, parent);
				sibling->info.is_black = true;
				parent->info.is_black = false;
				sibling = BSTSibling(child, parent);;
			}
			assert(sibling && sibling->info.is_black);
			if (parent->info.is_black && (!sibling->l || sibling->l->info.is_black) && (!sibling->r || sibling->r->info.is_black))
			{
				sibling->info.is_black = false;
				child = parent;
				parent = child->p;
				continue;
			}
			if (!parent->info.is_black && (!sibling->l || sibling->l->info.is_black) && (!sibling->r || sibling->r->info.is_black))
			{
				sibling->info.is_black = false;
				parent->info.is_black = true;
				return Root(parent);
			}
			if ((parent->l == child) && (!sibling->r || sibling->r->info.is_black))
			{
				assert(sibling->l && !sibling->l->info.is_black);
				BSTRotate<TNode, false>(sibling->l, sibling);
				sibling->info.is_black = false;
				sibling = sibling->p;
				sibling->info.is_black = true;
			}
			else if ((parent->r == child) && (!sibling->l || sibling->l->info.is_black))
			{
				assert(sibling->r && !sibling->r->info.is_black);
				BSTRotate<TNode, false>(sibling->r, sibling);
				sibling->info.is_black = false;
				sibling = sibling->p;
				sibling->info.is_black = true;
			}
			sibling->info.is_black = parent->info.is_black;
			parent->info.is_black = true;
			if (parent->l == child)
				sibling->r->info.is_black = true;
			else
				sibling->l->info.is_black = true;
			BSTRotate<TNode, true>(sibling, parent);
			return Root(sibling);
		}
		assert(false);
		return 0;
	}
};