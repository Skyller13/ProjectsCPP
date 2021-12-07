using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public struct Neighbors
{
	public Node Up { get; set; }
	public Node UpRight { get; set; }
	public Node Right { get; set; }
	public Node DownRight { get; set; }
	public Node Down { get; set; }
	public Node DownLeft { get; set; }
	public Node Left { get; set; }
	public Node UpLeft { get; set; }
}

[System.Serializable]
public class Node
{
	public enum NeighborsEnum
	{
		Up,
		UpRight,
		Right,
		DownRight,
		Down,
		DownLeft,
		Left,
		Upleft
	}
	private Neighbors neighbors;

	public Unit UnitOnThis = null;
	public SelectionQuad selectionQuad = null;

	public int TypeTile = 0; //To change for a enum like to the dictionary

	public int iGridX;//X Position in the Node Array
	public int iGridY;//Y Position in the Node Array

	public bool bIsWall;//Tells the program if this node is being obstructed.
	public Vector3 vPosition;//The world position of the node.

	private Node ParentNode;//For the AStar algoritm, will store what node it previously came from so it can trace the shortest path.

	public float igCost;//The cost of moving to the next square.
	public float ihCost;//The distance to the goal from this node.
	public float iwCost;

	public float FCost { get { return igCost + ihCost; } }//Quick get function to add G cost and H Cost, and since we'll never need to edit FCost, we dont need a set function.

	public Node GetNeighbor(NeighborsEnum _index)
	{
		switch (_index)
		{
			case NeighborsEnum.Up:
				return neighbors.Up;
			case NeighborsEnum.UpRight:
				return neighbors.UpRight;
			case NeighborsEnum.Right:
				return neighbors.Right;
			case NeighborsEnum.DownRight:
				return neighbors.DownRight;
			case NeighborsEnum.Down:
				return neighbors.Down; 
			case NeighborsEnum.DownLeft:
				return neighbors.DownLeft;
			case NeighborsEnum.Left:
				return neighbors.Left;
			case NeighborsEnum.Upleft:
				return neighbors.UpLeft;
			default:
				throw new ArgumentOutOfRangeException(nameof(_index), _index, null);
		}
	}
	
	public void SetNeighbors(NeighborsEnum _index, Node _node)
	{
		switch (_index)
		{
			case NeighborsEnum.UpRight:
				neighbors.UpRight = _node;
				break;
			case NeighborsEnum.Right:
				neighbors.Right = _node;
				break;
			case NeighborsEnum.DownRight:
				neighbors.DownRight = _node;
				break;
			case NeighborsEnum.Down:
				neighbors.Down = _node;
				break;
			case NeighborsEnum.DownLeft:
				neighbors.DownLeft = _node;
				break;
			case NeighborsEnum.Left:
				neighbors.Left = _node;
				break;
			case NeighborsEnum.Upleft:
				neighbors.UpLeft = _node;
				break;
			case NeighborsEnum.Up:
				neighbors.Up = _node;
				break;
			default:
				throw new ArgumentOutOfRangeException(nameof(_index), _index, null);
		}
	}

	public Node GetParent()
	{
		return ParentNode;
	}

	public void SetParent(Node _parent)
	{
		ParentNode = _parent;
	}
	
	public Node(bool a_bIsWall, int a_TypeTile, Vector3 a_vPos, int a_igridX, int a_igridY)//Constructor
	{
		TypeTile = a_TypeTile;

		iGridX = a_igridX;//X Position in the Node Array
		iGridY = a_igridY;//Y Position in the Node Array

		bIsWall = a_bIsWall;//Tells the program if this node is being obstructed.

		vPosition = a_vPos;//The world position of the node.

		iwCost = a_TypeTile;

		igCost = 0;
	}

	public Node(Node _newNode)
	{
		neighbors = _newNode.neighbors;

		UnitOnThis = _newNode.UnitOnThis;

		ParentNode = _newNode.ParentNode;

		bIsWall = _newNode.bIsWall;
		TypeTile = _newNode.TypeTile;

		vPosition = _newNode.vPosition;

		iGridX = _newNode.iGridX;
		iGridY = _newNode.iGridY;

		igCost = _newNode.igCost;
		ihCost = _newNode.ihCost;
	}


	public void WipeTheUnit()
	{
		if (UnitOnThis != null)
		{
			UnitOnThis = null;
		}
	}

	public void ChangeTheOccupant(Unit _newOccupant)
	{
		UnitOnThis = _newOccupant;
	}
}