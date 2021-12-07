using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PathFindingManager : MonoBehaviour
{
    DungeonCreator _dungeonCreator;

	public Node StartPosition;//Position de départ
	public Node TargetPosition;//Position d'arrivée

	public Node[,] tabNodePath;

	public List<Node> FinalPath;

	int xTabMap = 0;
	int yTabMap = 0;

	GameManager GM;
	bool IsInit = false;
	public void KapyStart(GameManager _GM)
	{
		GM = _GM;

		GM.ONManagersInitialized += Init;
	}

	private void OnDestroy()
	{
		GM.ONManagersInitialized -= Init;
	}

	public void Init()
	{
        _dungeonCreator = FindObjectOfType<DungeonCreator>();

        xTabMap = _dungeonCreator.GetWitdh();
        yTabMap = _dungeonCreator.GetHeight();

        tabNodePath = _dungeonCreator.GetNodeGrid();
		IsInit = true;
	}

    public Node GiveNode()
    {
        int x = Random.Range(0, 20);
        int y = Random.Range(0, 20);
    
        while (tabNodePath[y,x].bIsWall)
        {
            x = Random.Range(0, 20);
            y = Random.Range(0, 20);
        }

        return tabNodePath[y, x];
    }

	#region PathfindingFunctions
	public List<Node> FindPath(Node StartNode, Node TargetNode)
	{
		List<Node> OpenList = new List<Node>();//Liste des Nodes de la OpenList
		HashSet<Node> ClosedList = new HashSet<Node>();//Hashset de Nodes pour la ClosedList

		OpenList.Add(StartNode);//Ajoute la Node de départ à l'OpenList pour pouvoir commencer

		while (OpenList.Count > 0)//Tant qu'il y a quelque chose dans l'OpenList
		{
			Node CurrentNode = OpenList[0];//Créer une Node équivalente à la première de l'OpenList

			for (int i = 1; i < OpenList.Count; i++)//Parcours l'OpenList à partir de la 2e Node
			{
				if (OpenList[i].FCost < CurrentNode.FCost || OpenList[i].FCost == CurrentNode.FCost && OpenList[i].ihCost < CurrentNode.ihCost)//Si le fCost de cette Node est inférieur et ou égale au fCost de la CurrentNode
				{
					CurrentNode = OpenList[i];//Défini la CurrentNode par rapport à la Node
				}
			}

			OpenList.Remove(CurrentNode);//Supprime la CurrentNode de l'OpenList
			ClosedList.Add(CurrentNode);//Et l'ajoue à la ClosedList

            if (CurrentNode == TargetNode)//Si la CurrentNode est la même que notre Target
			{
				return GetFinalPath(StartNode, TargetNode);//On calcul le Chemin Final
			}

			foreach (Node NeighborNode in GetNeighboringNodes(CurrentNode))//Boucle pour tout les Voisins de la Node
			{

				if (NeighborNode.bIsWall || ClosedList.Contains(NeighborNode) || NeighborNode.UnitOnThis != null && NeighborNode != TargetNode)//Si le Voisin est un mur ou a déjà été vérifié
				{
					continue;//Le saute
				}

				float Start2Neighbor = CurrentNode.igCost + GetDistance(CurrentNode, NeighborNode) + (NeighborNode.iwCost) * 10;

				if (Start2Neighbor < NeighborNode.igCost || !OpenList.Contains(NeighborNode))//Si le fCost est plus grand que le gCost ou s'il n'est pas dans l'OpenList
				{
					NeighborNode.igCost = Start2Neighbor;
					NeighborNode.ihCost = GetDistance(NeighborNode, TargetNode);
					//---> On peut donc avoir le FCost
					NeighborNode.SetParent(CurrentNode);

					if (!OpenList.Contains(NeighborNode))//Si le Voisin n'est pas dans l'OpenList
					{
						OpenList.Add(NeighborNode);//L'ajoute à la l'OpenList
                       
                       
					}
				}
			}

		}

		Debug.Log("NULL");
		return null;
	}

	List<Node> GetFinalPath(Node a_StartingNode, Node a_EndNode)
	{
        List<Node> FinalPath = new List<Node>();//List du Chemin Final
		Node CurrentNode = a_EndNode;//CurrentNode pour stocker la Node en cours de vérification

		while (CurrentNode != a_StartingNode)//Boucle pour traverser chaque Node en passant par les parents jusqu'au Début du Chemin
		{
			FinalPath.Add(CurrentNode);//Ajoute cette Node au Chemin Final
            CurrentNode = CurrentNode.GetParent();//Déplace la CurrentNode au Parent de la Node
		}

		FinalPath.Reverse();//Inverse le Chemin pour l'avoir dans le bon ordre

		return FinalPath;//Défini le Chemin Final
	}

	public int GetDistance(Node a_nodeA, Node a_nodeB)
	{
		int dstX = Mathf.Abs(a_nodeA.iGridX - a_nodeB.iGridX);//x1-x2
		int dstY = Mathf.Abs(a_nodeA.iGridY - a_nodeB.iGridY);//y1-y2

		if (dstX > dstY)
		{
			return 14 * dstY + 10 * (dstX - dstY);
		}

		return 14 * dstX + 10 * (dstY - dstX);
	}

	public int GetManhattenDistance(Node a_nodeA, Node a_nodeB)
	{
		int ix = Mathf.Abs(a_nodeA.iGridX - a_nodeB.iGridX);//x1-x2
		int iy = Mathf.Abs(a_nodeA.iGridY - a_nodeB.iGridY);//y1-y2

		return ix + iy;//Renvoi la somme
	}

	int GetHeuristicDistance(Node a_nodeA, Node a_nodeB)
	{
		int ix = Mathf.Abs(a_nodeA.iGridX - a_nodeB.iGridX);//x1-x2
		int iy = Mathf.Abs(a_nodeA.iGridY - a_nodeB.iGridY);//y1-y2

		int min = Mathf.Min(ix, iy);
		int max = Mathf.Max(ix, iy);

		int diagonalSteps = min;
		int straightSteps = max - min;

		return diagonalSteps + straightSteps;
	}

	//Donne les Voisins d'une Node :
	public List<Node> GetNeighboringNodes(Node a_NeighborNode)
	{
		List<Node> NeighborList = new List<Node>();//Création d'une liste de Voisin de la Node
		int icheckX;//Check si XPosition est dans la range ou non.
		int icheckY;//Check si YPosition est dans la range ou non.

		//Check le côté Droit de la Node :
		icheckX = a_NeighborNode.iGridX + 1;
		icheckY = a_NeighborNode.iGridY;

		if (icheckX >= 0 && icheckX < xTabMap)//Si XPosition est dans la range du tableau
		{
			if (icheckY >= 0 && icheckY < yTabMap)//Si YPosition est dans la range du tableau
			{
				NeighborList.Add(tabNodePath[icheckX, icheckY]);//Ajoute la node à la liste des Voisins
			}
		}

		//Check le côté Gauche de la Node :
		icheckX = a_NeighborNode.iGridX - 1;
		icheckY = a_NeighborNode.iGridY;

		if (icheckX >= 0 && icheckX < xTabMap)//Si XPosition est dans la range du tableau
		{
			if (icheckY >= 0 && icheckY < yTabMap)//Si YPosition est dans la range du tableau
			{
				NeighborList.Add(tabNodePath[icheckX, icheckY]);//Ajoute la node à la liste des Voisins
			}
		}

		//Check le côté Haut de la Node :
		icheckX = a_NeighborNode.iGridX;
		icheckY = a_NeighborNode.iGridY + 1;

		if (icheckX >= 0 && icheckX < xTabMap)//Si XPosition est dans la range du tableau
		{
			if (icheckY >= 0 && icheckY < yTabMap)//Si YPosition est dans la range du tableau
			{
                NeighborList.Add(tabNodePath[icheckX, icheckY]);//Ajoute la node à la liste des Voisins
            }

		}

		//Check le côté Bas de la Node :
		icheckX = a_NeighborNode.iGridX;
		icheckY = a_NeighborNode.iGridY - 1;

		if (icheckX >= 0 && icheckX < xTabMap)//Si XPosition est dans la range du tableau
		{
			if (icheckY >= 0 && icheckY < yTabMap)//Si YPosition est dans la range du tableau
			{
                NeighborList.Add(tabNodePath[icheckX, icheckY]);//Ajoute la node à la liste des Voisins
            }
		}

		//Check le côté BasDroit de la Node :
		icheckX = a_NeighborNode.iGridX + 1;
		icheckY = a_NeighborNode.iGridY - 1;

		if (icheckX >= 0 && icheckX < xTabMap)//Si XPosition est dans la range du tableau
		{
			if (icheckY >= 0 && icheckY < yTabMap)//Si YPosition est dans la range du tableau
			{
				if (icheckX-1 >= 0 && icheckX-1 < xTabMap && icheckY+1 >= 0 && icheckY+1 < yTabMap)
				{
					if (!tabNodePath[icheckY, icheckX - 1].bIsWall && !tabNodePath[icheckY + 1, icheckX].bIsWall)
					{
						NeighborList.Add(tabNodePath[icheckX, icheckY]);//Ajoute la node à la liste des Voisins
					}
				}
				

			}
		}

		//Check le côté BasGauche de la Node :
		icheckX = a_NeighborNode.iGridX - 1;
		icheckY = a_NeighborNode.iGridY - 1;

		if (icheckX >= 0 && icheckX < xTabMap)//Si XPosition est dans la range du tableau
		{
			if (icheckY >= 0 && icheckY < yTabMap)//Si YPosition est dans la range du tableau
			{
				if (!tabNodePath[icheckY, icheckX + 1].bIsWall && !tabNodePath[icheckY + 1, icheckX].bIsWall)
				{
                    NeighborList.Add(tabNodePath[icheckX, icheckY]);//Ajoute la node à la liste des Voisins
                }
			}
		}

		//Check le côté HauDroit de la Node :
		icheckX = a_NeighborNode.iGridX + 1;
		icheckY = a_NeighborNode.iGridY + 1;

		if (icheckX >= 0 && icheckX < xTabMap)//Si XPosition est dans la range du tableau
		{
			if (icheckY >= 0 && icheckY < yTabMap)//Si YPosition est dans la range du tableau
			{
				if (!tabNodePath[icheckY, icheckX - 1].bIsWall && !tabNodePath[icheckY - 1, icheckX].bIsWall)
				{
                    NeighborList.Add(tabNodePath[icheckX, icheckY]);//Ajoute la node à la liste des Voisins
                }
			}
		}

		//Check le côté HautGauche de la Node :
		icheckX = a_NeighborNode.iGridX - 1;
		icheckY = a_NeighborNode.iGridY + 1;

		if (icheckX >= 0 && icheckX < xTabMap)//Si XPosition est dans la range du tableau
		{
			if (icheckY >= 0 && icheckY < yTabMap)//Si YPosition est dans la range du tableau
			{
				if (!tabNodePath[icheckY, icheckX + 1].bIsWall && !tabNodePath[icheckY - 1, icheckX].bIsWall)
				{
                    NeighborList.Add(tabNodePath[icheckX, icheckY]);//Ajoute la node à la liste des Voisins
                }
			}
		}

		return NeighborList;//Renvoi la liste de Voisin
	}
	#endregion

	#region RangeFunctions
	public List<Node> GetRange(Node _StartNode, int _Range)
	{
		List<Node> ClosedList = new List<Node>();
		List<Node> TamponList = new List<Node>();
		List<Node> OpenList = new List<Node>();

		TamponList.Add(_StartNode);
		
		for (int i = 0; i < _Range; i++)
		{
			foreach (var item in TamponList)
			{
				Node node = item.GetNeighbor(Node.NeighborsEnum.Up);
				if (node != null && !OpenList.Contains(node) && !node.bIsWall)
				{
					OpenList.Add(node);
				}
				node = item.GetNeighbor(Node.NeighborsEnum.UpRight);
				if (node != null && !OpenList.Contains(node) && !node.bIsWall)
				{
					OpenList.Add(node);
				}
				node = item.GetNeighbor(Node.NeighborsEnum.Right);
				if (node != null && !OpenList.Contains(node) && !node.bIsWall)
				{
					OpenList.Add(node);
				}
				node = item.GetNeighbor(Node.NeighborsEnum.DownRight);
				if (node != null && !OpenList.Contains(node) && !node.bIsWall)
				{
					OpenList.Add(node);
				}
				node = item.GetNeighbor(Node.NeighborsEnum.Down);
				if (node != null && !OpenList.Contains(node) && !node.bIsWall)
				{
					OpenList.Add(node);
				}
				node = item.GetNeighbor(Node.NeighborsEnum.DownLeft);
				if (node != null && !OpenList.Contains(node) && !node.bIsWall)
				{
					OpenList.Add(node);
				}
				node = item.GetNeighbor(Node.NeighborsEnum.Left);
				if (node != null && !OpenList.Contains(node) && !node.bIsWall)
				{
					OpenList.Add(node);
				}
				node = item.GetNeighbor(Node.NeighborsEnum.Upleft);
				if (node != null && !OpenList.Contains(node) && !node.bIsWall)
				{
					OpenList.Add(node);
				}
			}
			foreach (var item in TamponList)
			{
				ClosedList.Add(item);
			}
			foreach (var item in OpenList)
			{
				TamponList.Add(item);
			}
			OpenList.Clear();
		}

		foreach (var item in TamponList)
		{
			ClosedList.Add(item);
		}

		return ClosedList;

	}
	#endregion

	private void Update()
	{
		if (IsInit)
		{
			//!!! ATTENTION !!! Si tu dois mettre un update sur ce manager met tout dans ce if sinon on crash ^^ bisou =)
		}
	}
}
