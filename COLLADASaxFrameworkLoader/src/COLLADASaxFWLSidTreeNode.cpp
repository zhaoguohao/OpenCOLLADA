/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of COLLADASaxFrameworkLoader.

    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/

#include "COLLADASaxFWLStableHeaders.h"
#include "COLLADASaxFWLSidTreeNode.h"


namespace COLLADASaxFWL
{


	//------------------------------
	bool SidTreeNode::SidIdentifier::operator<( const SidIdentifier& rhs ) const
	{
		if ( sid < rhs.sid)
			return true;

		if ( sid > rhs.sid)
			return false;

		if ( hierarchyLevel < rhs.hierarchyLevel)
			return true;

		if ( hierarchyLevel > rhs.hierarchyLevel)
			return false;

		return false;
	}

	//------------------------------
	SidTreeNode::SidTreeNode(const String& sid, SidTreeNode *parent)
		: mParent(parent)
		, mTargetType(TARGETTYPE_UNKNOWN)
		, mSid(sid)
	{
		mTarget.object = 0;
	}

	//------------------------------
	SidTreeNode::~SidTreeNode()
	{
	}

	//------------------------------
	SidTreeNode* SidTreeNode::createAndAddChild( const String& sid )
	{
		SidTreeNode* newChild = new SidTreeNode(sid, this);
		if ( !sid.empty() )
		{
			SidIdentifier newIdentifier(newChild->getSid(), 0);
			mChildren.insert(std::make_pair(newIdentifier, newChild));
			addChildToParents( newChild, newIdentifier );
		}
		return newChild;
	}

	//------------------------------
	void SidTreeNode::addChildToParents( SidTreeNode *sidTreeNode, const SidIdentifier& sidIdentifier )
	{
		if ( sidTreeNode )
		{
			SidTreeNode *parent = sidTreeNode->getParent();
			if ( parent )
			{
				SidIdentifier parentSidIdentifier = sidIdentifier;
				// if the parent has no sid, i.e. it has an id, we don't increase the hierarchy level.*/
				if ( !parent->getSid().empty() )
				{
					parentSidIdentifier.hierarchyLevel++;
				}
				mChildren.insert(std::make_pair(parentSidIdentifier, parent));
				addChildToParents( parent, parentSidIdentifier );
			}
		}
	}

	//------------------------------
	SidTreeNode* SidTreeNode::findChildBySid( const String& sid )
	{
		SidIdentifier lowerBound( sid, 0);
		SidIdentifierSidTreeNodeMap::iterator it = mChildren.lower_bound( lowerBound );

		if ( it == mChildren.end() )
			return 0;

		SidIdentifier canidate = it->first;
		if ( canidate.sid == sid )
		{
			return it->second;
		}
		else
		{
			return 0;
		}
	}

} // namespace COLLADASaxFWL
