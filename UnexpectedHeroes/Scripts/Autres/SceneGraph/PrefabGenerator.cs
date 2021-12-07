using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEngine;
using SimpleJSON;
#if UNITY_EDITOR
using UnityEditor;
#endif

public class PrefabGenerator : MonoBehaviour
{
    [Header("Pour vous les graphs")]
    [Tooltip("Nom du prefab lorsqu'il sera sauvegardé.")]
    public string roomName;

    [Tooltip("Le fichier room prérempli.")]
    public ScriptableRoom roomFile;
    
    private Vector2Int m_vMapSize;
    private int[,,] m_iMapGrid;
    private int m_nbLayer;
    private int m_groundLayer;

    private Dictionary<int, Material> m_dicoUpTileMaterials = new Dictionary<int, Material>();
    private Dictionary<int, Material> m_dicoSideTileMaterials = new Dictionary<int, Material>();

    private Quaternion leftTileRotation;
    private Quaternion rightTileRotation;
    private Quaternion frontTileRotation;
    private Quaternion backTileRotation;

    private Vector3 rightTileOffset = new Vector3(0.5f, -0.5f, 0f);
    private Vector3 leftTileOffset = new Vector3(-0.5f, -0.5f, 0f);
    private Vector3 frontTileOffset = new Vector3(0, -0.5f, 0.5f);
    private Vector3 backTileOffset = new Vector3(0, -0.5f, -0.5f);

    private GameObject room;
    private GameObject planeHolder;
    
    
    //private Room roomScriptable = new Room();
    
    [Header("Meshes & Material")]
    public Mesh m_meshTileUp;
    public Material defaultMaterial;
    public Material fondMarinMat;
    public Material underAnotherBlockMat;
    

    private void Awake()
    {
        room = new GameObject(roomName);
        planeHolder = new GameObject("Cube Holder");
        planeHolder.transform.parent = room.transform;
        
        leftTileRotation = Quaternion.Euler(90f,0f,90f);
        rightTileRotation = Quaternion.Euler(90f,0f,-90f);
        frontTileRotation = Quaternion.Euler(90f,0f,0f);
        backTileRotation = Quaternion.Euler(90f,0f,-180f);
        
        foreach (var roomMaterial in roomFile.roomMaterials)
        {
            m_dicoUpTileMaterials.Add(roomMaterial.id, roomMaterial.upTileMaterial);
            m_dicoSideTileMaterials.Add(roomMaterial.id, roomMaterial.sideTileMaterial);
        }
    }

    private void Start()
    {
        LoadMap();

        SetSize();
        
        SaveGrid();
        
        GenerateRoom();

        #if UNITY_EDITOR
        PrefabUtility.SaveAsPrefabAsset(room, Application.dataPath + "/Prefabs/Ressources/Rooms/" + roomName + ".prefab");
        #endif
    }

    /// <summary>
    /// Sauvegarde la grille dans un fichier.txt
    /// </summary>
    private void SaveGrid()
    {
        
        var roomComponent = room.AddComponent<Room>(); 
        roomComponent.m_typeGrid = new int[m_vMapSize.x * m_vMapSize.y];
        for (int i = 0; i < m_vMapSize.x; i++)
        {
            for (int j = 0; j < m_vMapSize.y; j++)
            {
                roomComponent.m_typeGrid[m_vMapSize.x * i + j] = m_iMapGrid[m_groundLayer, i, j];
            }
        }
        
        // var tempString = String.Join(" ", roomScriptable.m_typeGrid.Select(p => p.ToString()).ToArray());
        //
        // File.WriteAllText(Application.dataPath + "/Resources/Rooms/" + roomName + ".txt", tempString);
        //
        // roomComponent.m_textFile = Resources.Load<TextAsset>("Rooms/" + room.gameObject.transform.name);
        // roomComponent.m_typeGrid = roomComponent.m_textFile.text.Split(' ').Select(n => Convert.ToInt32(n)).ToArray();
    }

    private void LoadMap()
    {
        var root = JSON.Parse(roomFile.mapFile.text);
        
        // Layers
        var layers = root["layers"];
        m_nbLayer = layers.Count;
        var layer = layers[0];
        m_vMapSize.x = layer["width"].AsInt;
        m_vMapSize.y = layer["height"].AsInt;
        m_iMapGrid = new int[m_nbLayer,m_vMapSize.x,m_vMapSize.y];
        for (int i = 0; i < m_nbLayer; i++)
        {
            if (layers[i]["name"] == "ground")
            {
                m_groundLayer = i;
            }
            for (var j = 0; j < m_vMapSize.x; ++j)
            {
                for (var k = 0; k < m_vMapSize.y; ++k)
                {
                    m_iMapGrid[i,k,j] = layers[i]["data"][k * m_vMapSize.x + j].AsInt;
                }
            }
        }
        
        //Tiles ///INUTILE POUR LE MOMENT ! A DEPLACER LORS DE LA GENERATION DU DONJON ! \\
        // var tilesets = root["tilesets"];
        //
        // for (int iTileset = 0; iTileset < tilesets.Count; ++iTileset)
        // {
        //     var pTileset = new Tileset();
        //     var map = new Room();
        //     var tileset = tilesets[iTileset];
        //     var tiles = tileset["tiles"];
        //     for (int iTiles = 0; iTiles < tiles.Count; iTiles++)
        //     {
        //         var tile = tiles[iTiles];
        //         var newTile = new Tile();
        //         newTile.SetName(tile["type"]);
        //         
        //         var properties = tile["properties"];
        //         for (int iProperty = 0; iProperty < properties.Count; ++iProperty)
        //         {
        //             var property = properties[iProperty];
        //             newTile.SetCost(property["value"].AsInt);
        //         }
        //         pTileset.m_pTiles.Add(newTile);
        //     }
        //
        //     m_listTileset.Add(pTileset);
        // }
    }
    
    ///INUTILE POUR LE MOMENT ! A DEPLACER LORS DE LA GENERATION DU DONJON ! \\
    private void SetSize()
    {
        // m_Grid = new Node[m_vMapSize.x,m_vMapSize.y];
        //
        // for (int i = 0; i < m_vMapSize.x; i++)
        // {
        //     for (int j = 0; j < m_vMapSize.y; j++)
        //     {
        //         m_Grid[i, j] = new Node((m_iMapGrid[0, i, j] == 0),m_iMapGrid[0,i,j], new Vector3(i, 0, j), i, j);
        //         if (j != m_vMapSize.y - 1)
        //         {
        //             m_Grid[i,j].neighbors.Up = m_Grid[i, j + 1]; // top node
        //         }
        //         if (j != m_vMapSize.y - 1 && i != m_vMapSize.x - 1)
        //         {
        //             m_Grid[i,j].neighbors.UpRight = m_Grid[i + 1,j + 1]; // top-right node
        //         }
        //         if (i != m_vMapSize.x - 1)
        //         {
        //             m_Grid[i,j].neighbors.Right = m_Grid[i + 1,j]; // right node
        //         }
        //         if (i != m_vMapSize.x - 1 && j != 0)
        //         {
        //             m_Grid[i,j].neighbors.DownRight = m_Grid[i + 1,j - 1]; // bottom-right node
        //         }
        //         if (j != 0)
        //         {
        //             m_Grid[i,j].neighbors.Down = m_Grid[i,j - 1]; // bottom node
        //         }
        //         if (i != 0 && j != 0)
        //         {
        //             m_Grid[i,j].neighbors.DownLeft = m_Grid[i - 1,j - 1]; // bottom-left node
        //         }
        //         if (i != 0)
        //         {
        //             m_Grid[i,j].neighbors.Left = m_Grid[i - 1,j]; // left node
        //         }
        //         if (i != 0 && j != m_vMapSize.y - 1)
        //         {
        //             m_Grid[i,j].neighbors.UpLeft = m_Grid[i - 1,j + 1]; // top-left node
        //         }
        //     }
        // }
    }

    /// <summary>
    /// Génère le prefab 
    /// </summary>
    private void GenerateRoom()
    {
        for (int i = 0; i < m_nbLayer; i++)
        {
            for (int j = 0; j < m_vMapSize.x; j++)
            {
                for (int k = 0; k < m_vMapSize.y; k++)
                {
                    if (m_iMapGrid[i,j,k] != 0)
                    {
                        var cube = new GameObject("Cube");
                        cube.transform.parent = planeHolder.transform;
                        var position = new Vector3(j, i - m_groundLayer, k);
                        var waterOffset = new Vector3(0f,0.2f,0f);
                        
                        if (m_iMapGrid[i,j,k]==5)
                        {
                            position -= waterOffset;
                        }
                        cube.transform.position = position;

                        if (j == 0)
                        {
                            GenerateTileLeft(i,j, k, cube.transform);
                        }
                        else
                        {
                            if (m_iMapGrid[i, j - 1, k] == 0 || (m_iMapGrid[i, j - 1, k] == 5 && m_iMapGrid[i, j, k] !=5))
                            {
                                GenerateTileLeft(i,j, k,cube.transform);
                            }
                        }

                        if (j == m_vMapSize.x - 1)
                        {
                            GenerateTileRight(i,j, k,cube.transform);
                        }
                        else
                        {
                            if (m_iMapGrid[i,j + 1, k] == 0 || (m_iMapGrid[i, j + 1, k] == 5 && m_iMapGrid[i, j, k] !=5))
                            {
                                GenerateTileRight(i,j, k,cube.transform);
                            }
                        }

                        if (k == 0)
                        {
                            GenerateTileBack(i,j, k,cube.transform);
                        }
                        else
                        {
                            if (m_iMapGrid[i,j, k - 1] == 0 || (m_iMapGrid[i, j, k - 1] == 5 && m_iMapGrid[i, j, k] !=5))
                            {
                                GenerateTileBack(i,j, k,cube.transform);
                            }
                        }

                        if (k == m_vMapSize.y - 1)
                        {
                            GenerateTileFront(i,j, k,cube.transform);
                        }
                        else
                        {
                            if (m_iMapGrid[i, j,k + 1] == 0 || (m_iMapGrid[i, j, k + 1] == 5 && m_iMapGrid[i, j, k] !=5))
                            {
                                GenerateTileFront(i,j, k,cube.transform);
                            }
                        }

                        GenerateTileUp(i, j, k,cube.transform);
                    }
                }
            }
        }
    }

    private void GenerateTileUp(int _i, int _j,int _k, Transform _parent)
    {
        //hello
        var dalle = new GameObject("Tile Up");
        dalle.transform.parent = _parent;
        dalle.transform.position = _parent.position;
        dalle.AddComponent<MeshFilter>().mesh = m_meshTileUp;
        if (m_iMapGrid[_i,_j,_k] == 5)
        {
            var fondMarin = new GameObject("FondMarin");
            fondMarin.transform.parent = _parent;
            fondMarin.transform.position = _parent.position - Vector3.up + new Vector3(0,.2f,0);
            fondMarin.AddComponent<MeshFilter>().mesh = m_meshTileUp;
            fondMarin.AddComponent<MeshRenderer>().material = fondMarinMat;
        }
        Material material;
        if (m_iMapGrid[_i,_j,_k] == 1 || m_iMapGrid[_i,_j,_k] == 2)
        {
            if (_j>0)
            {
                if (!m_dicoUpTileMaterials.TryGetValue(m_iMapGrid[_i, _j - 1, _k],
                    out material))
                {
                    material = defaultMaterial;
                }
                dalle.AddComponent<MeshRenderer>().material = material;
            }
            else if (_j < m_vMapSize.y)
            {
                if (!m_dicoUpTileMaterials.TryGetValue(m_iMapGrid[_i, _j + 1, _k], out material))
                {
                    material = defaultMaterial;
                }
                dalle.AddComponent<MeshRenderer>().material = material;
            }
            else if (_k > 0)
            {
                if (!m_dicoUpTileMaterials.TryGetValue(m_iMapGrid[_i, _j, _k - 1], out material))
                {
                    material = defaultMaterial;
                }
                dalle.AddComponent<MeshRenderer>().material = material;
            }
            else if (_k < m_vMapSize.x)
            {
                if (!m_dicoUpTileMaterials.TryGetValue(m_iMapGrid[_i, _j, _k + 1], out material))
                {
                    material = defaultMaterial;
                }
                dalle.AddComponent<MeshRenderer>().material = material;
            }
            else
            {
                Debug.LogError("Sortie de la grille lors du placement d'une Tile Up");
            }
        }
        else
        {
            if (!m_dicoUpTileMaterials.TryGetValue(m_iMapGrid[_i, _j, _k], out material))
            {
                material = defaultMaterial;
            }
            dalle.AddComponent<MeshRenderer>().material = material;
        }
    }

    private void GenerateTileLeft(int _i, int _j,int _k, Transform _parent)
    {
        var dalleLeft = new GameObject("Tile Left");
        dalleLeft.transform.parent = _parent;
        dalleLeft.transform.rotation = leftTileRotation;
        dalleLeft.transform.position = _parent.position + leftTileOffset;
        dalleLeft.AddComponent<MeshFilter>().mesh = m_meshTileUp;
        var meshRenderer = dalleLeft.AddComponent<MeshRenderer>();
        Material material;
        if (m_iMapGrid[_i, _j, _k] == 1 || m_iMapGrid[_i, _j, _k] == 2) 
        {
            if (_j>0)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j - 1, _k], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else if (_j < m_vMapSize.y)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j + 1, _k], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else if (_k > 0)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j, _k - 1], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else if (_k < m_vMapSize.x)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j, _k + 1], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else
            {
                Debug.LogError("Sortie de la grille lors du placement d'une Tile Left");
            }
        }
        else
        {
            if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j, _k], out material))
            {
                material = defaultMaterial;
            }
            if (_i < m_nbLayer - 1)
            {
                if (m_iMapGrid[_i + 1,_j,_k] != 0)
                {
                    material = underAnotherBlockMat;
                }
            }
            meshRenderer.material = material;
        }
    }

    private void GenerateTileRight(int _i, int _j,int _k, Transform _parent)
    {
        var dalleRight = new GameObject("Tile Right");
        dalleRight.transform.parent = _parent;
        dalleRight.transform.rotation = rightTileRotation;
        dalleRight.transform.position = _parent.position + rightTileOffset;
        dalleRight.AddComponent<MeshFilter>().mesh = m_meshTileUp;
        var meshRenderer = dalleRight.AddComponent<MeshRenderer>();
        Material material;
        if (m_iMapGrid[_i,_j,_k] == 1 || m_iMapGrid[_i,_j,_k] == 2)
        {
            if (_j>0)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j - 1, _k], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else if (_j < m_vMapSize.y)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j + 1, _k], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else if (_k > 0)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j, _k - 1], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else if (_k < m_vMapSize.x)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j, _k + 1], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else
            {
                Debug.LogError("Sortie de la grille lors du placement d'une Tile Right");
            }
        }
        else
        {
            if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j, _k], out material))
            {
                material = defaultMaterial;
            }
            if (_i < m_nbLayer - 1)
            {
                if (m_iMapGrid[_i + 1,_j,_k] != 0)
                {
                    material = underAnotherBlockMat;
                }
            }
            meshRenderer.material = material;
        }
    }

    private void GenerateTileFront(int _i, int _j,int _k, Transform _parent)
    {
        var dalleFront = new GameObject("Tile Front");
        dalleFront.transform.parent = _parent;
        dalleFront.transform.rotation = frontTileRotation;
        dalleFront.transform.position = _parent.position + frontTileOffset;
        dalleFront.AddComponent<MeshFilter>().mesh = m_meshTileUp;
        var meshRenderer = dalleFront.AddComponent<MeshRenderer>();
        Material material;
        if (m_iMapGrid[_i,_j,_k] == 1 || m_iMapGrid[_i,_j,_k] == 2)
        {
            if (_j>0)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j - 1, _k], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else if (_j < m_vMapSize.y)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j + 1, _k], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else if (_k > 0)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j, _k - 1], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else if (_k < m_vMapSize.x)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j, _k + 1], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else
            {
                Debug.LogError("Sortie de la grille lors du placement d'une Tile Front");
            }
        }
        else
        {
            if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j, _k], out material))
            {
                material = defaultMaterial;
            }
            if (_i < m_nbLayer - 1)
            {
                if (m_iMapGrid[_i + 1,_j,_k] != 0)
                {
                    material = underAnotherBlockMat;
                }
            }
            meshRenderer.material = material;
        }
    }

    private void GenerateTileBack(int _i, int _j,int _k, Transform _parent)
    {
        var dalleBack = new GameObject("Tile Back");
        dalleBack.transform.parent = _parent;
        dalleBack.transform.rotation = backTileRotation;
        dalleBack.transform.position = _parent.position + backTileOffset;
        dalleBack.AddComponent<MeshFilter>().mesh = m_meshTileUp;
        var meshRenderer = dalleBack.AddComponent<MeshRenderer>();
        Material material;
        if (m_iMapGrid[_i,_j,_k] == 1 || m_iMapGrid[_i,_j,_k] == 2)
        {
            if (_j>0)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j - 1, _k], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else if (_j < m_vMapSize.y)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j + 1, _k], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else if (_k > 0)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j, _k - 1], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else if (_k < m_vMapSize.x)
            {
                if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j, _k + 1], out material))
                {
                    material = defaultMaterial;
                }
                meshRenderer.material = material;
            }
            else
            {
                Debug.LogError("Sortie de la grille lors du placement d'une Tile Back");
            }
        }
        else
        {
            if (!m_dicoSideTileMaterials.TryGetValue(m_iMapGrid[_i, _j, _k], out material))
            {
                material = defaultMaterial;
            }
            if (_i < m_nbLayer - 1)
            {
                if (m_iMapGrid[_i + 1,_j,_k] != 0)
                {
                    material = underAnotherBlockMat;
                }
            }
            meshRenderer.material = material;
        }
    }
}