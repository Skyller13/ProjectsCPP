using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEngine;
using UnityEngine.SceneManagement;


public class DungeonCreator : MonoBehaviour
{
    protected GameManager gameManager;
    public GameObject selectionQuad; 
    
    [Header("Dungeon specs")]
    public GameObject player;
    public GameObject enemy;
    public int m_iWidth;
    public int m_iHeight;
    public GameObject[,] m_pMaps;
    private Node[,] m_nodeGrid;
    private bool hasPlayerSpawned;
    private bool hasEnemySpawned;
    public bool setupHasBeenDone;
    
    [Header("Room DB")]
    public int MAX_NB_OF_ROOMS;
    public int roomSpawned;
    public GameObject initialRoom;
    public GameObject topTeleporter;
    public GameObject bottomTeleporter;
    public GameObject leftTeleporter;
    public GameObject rightTeleporter;
    public GameObject GD;
    public GameObject GB;
    public GameObject[] bottomRooms;
    public GameObject[] topRooms;
    public GameObject[] leftRooms;
    public GameObject[] rightRooms;

    public List<GameObject> spawnedRooms;

    public int[,] m_iGrid;

    public event VoidDelegate canISetup;

    public Node[,] GetNodeGrid()
    {
        return m_nodeGrid;
    }

    public int GetWitdh()
    {
        return m_iWidth;
    }
    public int GetHeight()
    {
        return m_iHeight;
    }
    
    private void Start()
    {
        gameManager = Toolbox.Instance.GetManager<GameManager>();
        spawnedRooms.Add(Instantiate(initialRoom, Vector3.zero, Quaternion.identity));
        roomSpawned++;

        canISetup += SetupDungeon;
    }

    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.Keypad0))
        {
            SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex, LoadSceneMode.Single);
        }
    }

    public void LaunchEvent()
    {
        canISetup?.Invoke();
    }

    private void SetupDungeon()
    {
        var minX = int.MaxValue;
        var maxX = int.MinValue;
        var minZ = int.MaxValue;
        var maxZ = int.MinValue;

        // Permet de trouver la position en X et en Z la plus petite pour recaler les maps
        foreach (var templatesSpawnedRoom in spawnedRooms)
        {
            var position = templatesSpawnedRoom.transform.position;
            minX = (int)Mathf.Min(position.x, minX);
            minZ = (int)Mathf.Min(position.z, minZ);
        }

        var offset = new Vector3(Mathf.Abs(minX),0, Mathf.Abs(minZ));
        
        // Recale la map puis trouve la position en X et en Z la plus grande pour trouver la taille du donjon
        foreach (var templatesSpawnedRoom in spawnedRooms)
        {
            var position = templatesSpawnedRoom.transform.position;
            position += offset;
            templatesSpawnedRoom.transform.position = position;
            maxX = (int) Mathf.Max(position.x + 40, maxX);
            maxZ = (int) Mathf.Max(position.z + 40, maxZ);
        }

        m_iWidth = maxX;
        m_iHeight = maxZ;
        m_pMaps = new GameObject[m_iWidth/40,m_iHeight/40];
        
        // Rempli m_pMaps avec les différentes salles selon leur position
        foreach (var templatesSpawnedRoom in spawnedRooms)
        {
            var position = templatesSpawnedRoom.transform.position;
            var indexX = (int)position.x / 40;
            var indexZ = (int)position.z / 40;
            m_pMaps[indexX,indexZ] = templatesSpawnedRoom;
        }
        
        GenerateDungeonGrid();
    }
    
    private void GenerateDungeonGrid()
    {
        m_iGrid = new int[m_iWidth,m_iHeight];

        // i = width index
        // j = height index
        for (int i = 0; i < m_pMaps.GetLength(0); i++)
        {
            for (int j = 0; j < m_pMaps.GetLength(1); j++)
            {
                var offsetX = i * 40;
                var offsetZ = j * 40;
                for (int height = 0; height < 40; height++)
                {
                    for (int width = 0; width < 40; width++)
                    {
                        if (m_pMaps[i,j] == null)
                        {
                            m_iGrid[width + offsetX, height + offsetZ] = 0;
                        }
                        else
                        {
                            m_iGrid[width + offsetX, height + offsetZ] = m_pMaps[i,j].GetComponent<Room>().m_typeGrid[width * 40 + height];
                        }
                    }
                }
            }
        }

        m_nodeGrid = SetNode();

        
        
        for (int i = 0; i < m_iGrid.GetLength(0); i++)
        {
            for (int j = 0; j < m_iGrid.GetLength(1); j++)
            {
                // Spawn des quads
                if (!m_nodeGrid[i,j].bIsWall)
                {
                    var pos = m_nodeGrid[i,j].vPosition;
                    pos.y += .01f;
                    var quad = Instantiate(selectionQuad, pos, Quaternion.identity);
                    quad.transform.Rotate(new Vector3(90, 0, 90));
                    quad.transform.parent = gameObject.transform;
                    m_nodeGrid[i, j].selectionQuad = quad.GetComponent<SelectionQuad>();
                }


                // Spawn du joueur
                if (m_iGrid[i,j] == 1 && !hasPlayerSpawned)
                {
                    m_iGrid[i, j] = m_iGrid[i, j - 1];
                    var playerGameObject = Instantiate(player, new Vector3(i, 1, j), Quaternion.identity);
                    var playerComponent = playerGameObject.GetComponent<Unit>();
                    playerComponent.PosX = i;
                    playerComponent.PosY = j;
                    playerComponent.SetCurrentNodePose(m_nodeGrid[i,j]);
                    m_nodeGrid[i, j].UnitOnThis = playerComponent;
                    hasPlayerSpawned = true;
                }

                // Spawn des ennemis
                if (m_iGrid[i,j] == 2)
                {
                    m_iGrid[i, j] = m_iGrid[i, j - 1];
                    var enemyGameObject = Instantiate(enemy, new Vector3(i, 1, j), Quaternion.identity);
                    var enemyComponent = enemyGameObject.GetComponent<Enemy>();
                    enemyComponent.PosX = i;
                    enemyComponent.PosY = j;
                    enemyComponent.SetCurrentNodePose(m_nodeGrid[i,j]);
                    m_nodeGrid[i, j].UnitOnThis = enemyComponent;
                }
            }
        }

        if (!hasPlayerSpawned)
        {
            var x = 130 / 2;
            var y = 130 / 2;
            m_iGrid[x,y] = m_iGrid[x, y - 1];
            var playerGameObject = Instantiate(player, new Vector3(x, 1, y), Quaternion.identity);
            var playerComponent = playerGameObject.GetComponent<Unit>();
            playerComponent.PosX = x;
            playerComponent.PosY = y;
            playerComponent.SetCurrentNodePose(m_nodeGrid[x,y]);
            m_nodeGrid[x,y].UnitOnThis = playerComponent;
            hasPlayerSpawned = true;
        }
        
        // //Print la grille du donjon dans un TXT, DEBUG ONLY ENTRAINE UN FREEZE
        // var s = "";
        // for (int i = 0; i < m_iGrid.GetLength(0); i++) {
        //     if (i > 0) s += ' ';
        //     s += "\n";
        //     for (int j = 0; j < m_iGrid.GetLength(1); j++) {
        //         if (m_iGrid[i,j] != 0)
        //         {
        //             s += 1;
        //         }
        //         else
        //         {
        //             s += "0";
        //         }
        //     }
        // }
        // File.WriteAllText(Application.dataPath + "/Sandbox/GridDuDonjon.txt", s);
        gameManager.InitDonjonFloor();
    }

    public Node[,] SetNode()
    {
        m_nodeGrid = new Node[m_iWidth,m_iHeight];
        
        for (int i = 0; i < m_iWidth; i++)
        {
            for (int j = 0; j < m_iHeight; j++)
            {
                m_nodeGrid[i, j] = new Node((m_iGrid[i, j] == 0 || m_iGrid[i,j] == 8 || m_iGrid[i,j] == 5),m_iGrid[i,j], new Vector3(i, 0, j), i, j);
            }
        }

        for (int i = 0; i < m_iWidth; i++)
        {
            for (int j = 0; j < m_iHeight; j++)
            {
                
                if (j != m_iHeight - 1)
                {
                    m_nodeGrid[i,j].SetNeighbors(Node.NeighborsEnum.Up, m_nodeGrid[i,j+1]); // top node
                }
                if (j != m_iHeight - 1 && i != m_iWidth - 1)
                {
                    m_nodeGrid[i,j].SetNeighbors(Node.NeighborsEnum.UpRight, m_nodeGrid[i + 1, j + 1]); // top-right node
                }
                if (i != m_iWidth - 1)
                {
                    m_nodeGrid[i,j].SetNeighbors(Node.NeighborsEnum.Right, m_nodeGrid[i + 1, j]); // right node
                }
                if (i != m_iWidth - 1 && j != 0)
                {
                    m_nodeGrid[i,j].SetNeighbors(Node.NeighborsEnum.DownRight, m_nodeGrid[i + 1, j - 1]); // bottom-right node
                }
                if (j != 0)
                {
                    m_nodeGrid[i,j].SetNeighbors(Node.NeighborsEnum.Down, m_nodeGrid[i, j - 1]); // bottom node
                }
                if (i != 0 && j != 0)
                {
                    m_nodeGrid[i,j].SetNeighbors(Node.NeighborsEnum.DownLeft, m_nodeGrid[i - 1, j - 1]); // bottom-left node
                }
                if (i != 0)
                {
                    m_nodeGrid[i, j].SetNeighbors(Node.NeighborsEnum.Left, m_nodeGrid[i - 1, j]); // left node
                }
                if (i != 0 && j != m_iHeight - 1)
                {
                    m_nodeGrid[i,j].SetNeighbors(Node.NeighborsEnum.Upleft, m_nodeGrid[i - 1, j + 1]); // top-left node
                }
            }
        }
        
        return m_nodeGrid;
    }
}

public class Tileset
{
    public List<Tile> m_pTiles;
}

public class Tile
{
    [Serializable]
    struct TileInfo
    {
        public string szName;
        public int iCost;
    }

    private TileInfo m_tileInfo;

    /// <summary>
    /// Set the name of the current tile
    /// </summary>
    /// <param name="_szName"></param>
    public void SetName(string _szName)
    {
        m_tileInfo.szName = _szName;
    }

    /// <summary>
    /// Return the name of the current tile
    /// </summary>
    /// <returns></returns>
    public string GetName()
    {
        return m_tileInfo.szName;
    }

    /// <summary>
    /// Set the cost of the current tile
    /// -1 => void
    /// </summary>
    /// <param name="_iCost"></param>
    public void SetCost(int _iCost)
    {
        m_tileInfo.iCost = _iCost;
    }

    /// <summary>
    /// Return the cost of the current tile
    /// </summary>
    /// <returns></returns>
    public int GetCost()
    {
        return m_tileInfo.iCost;
    }
}