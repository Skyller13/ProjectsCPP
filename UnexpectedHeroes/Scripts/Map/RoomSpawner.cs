using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RoomSpawner : MonoBehaviour
{
    public int openingDirection;
    //1 : top
    //2 : bottom
    //3 : left
    //4 : right

    private DungeonCreator dungeon;
    private int rand;
    private bool hasSpawned;

    private void Awake()
    {
        Destroy(gameObject, 4f);
        dungeon = FindObjectOfType<DungeonCreator>();

        Invoke(nameof(Spawn), 0.2f);
    }

    private void Spawn()
    {
        if (!hasSpawned)
        {
            if (dungeon.roomSpawned < dungeon.MAX_NB_OF_ROOMS - 1)
            {
                GameObject spawnedRoom = null;
                switch (openingDirection)
                {
                    case 1: //Spawn a room with a bottom door
                        rand = UnityEngine.Random.Range(0, dungeon.bottomRooms.Length);
                        spawnedRoom = Instantiate(dungeon.bottomRooms[rand], transform.position, Quaternion.identity);
                        break;
                    case 2: //Spawn a room with a top door
                        rand = UnityEngine.Random.Range(0, dungeon.topRooms.Length);
                        spawnedRoom = Instantiate(dungeon.topRooms[rand], transform.position, Quaternion.identity);
                        break;
                    case 3: //Spawn a room with a right door
                        rand = UnityEngine.Random.Range(0, dungeon.rightRooms.Length);
                        spawnedRoom = Instantiate(dungeon.rightRooms[rand], transform.position, Quaternion.identity);
                        break;
                    case 4: //Spawn a room with a left door
                        rand = UnityEngine.Random.Range(0, dungeon.leftRooms.Length);
                        spawnedRoom = Instantiate(dungeon.leftRooms[rand], transform.position, Quaternion.identity);
                        break;
                    default:
                        Debug.LogError("openingDirection should be [1;4].");
                        break;
                }

                dungeon.spawnedRooms.Add(spawnedRoom);
                dungeon.roomSpawned++;

            }
            else if (dungeon.roomSpawned == dungeon.MAX_NB_OF_ROOMS - 1)
            {
                GameObject spawnedRoom = null;
                var position = transform.position;
                switch (openingDirection)
                {
                    case 1: //Need bottom
                        spawnedRoom = Instantiate(dungeon.bottomTeleporter, position, Quaternion.identity);
                        break;
                    case 2: //Need top
                        spawnedRoom = Instantiate(dungeon.topTeleporter, position, Quaternion.identity);
                        break;
                    case 3: //Need right
                        spawnedRoom = Instantiate(dungeon.rightTeleporter, position, Quaternion.identity);
                        break;
                    case 4: //Need left
                        spawnedRoom = Instantiate(dungeon.leftTeleporter, position, Quaternion.identity);
                        break;
                    default:
                        Debug.LogError("openingDirection should be [1;4].");
                        break;
                }


                dungeon.spawnedRooms.Add(spawnedRoom);
                dungeon.roomSpawned++;
            }

            hasSpawned = true;

            if (dungeon.roomSpawned == dungeon.MAX_NB_OF_ROOMS && !dungeon.setupHasBeenDone)
            {
                dungeon.setupHasBeenDone = true;
                dungeon.LaunchEvent();
            }

            // donjon vas-y execute ton start
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("SpawnPoint"))
        {
            // GameObject spawnedRoom = null;
            // var roomSpawner = other.GetComponent<RoomSpawner>();
            // if (roomSpawner.hasSpawned == false && hasSpawned == false)
            // {
            //     //Top
            //     if (roomSpawner.openingDirection == 1)
            //     {
            //         switch (openingDirection)
            //         {
            //             //Bottom
            //             case 2:
            //                 break;
            //             //Left
            //             case 3:
            //                 break;
            //             //Right
            //             case 4:
            //                 spawnedRoom = Instantiate(dungeon.GB, transform.position, Quaternion.identity);
            //                 dungeon.spawnedRooms.Add(spawnedRoom);
            //                 dungeon.roomSpawned++;
            //                 break;
            //         }
            //     }
            //     //Bottom
            //     else if (roomSpawner.openingDirection == 2)
            //     {
            //         switch (openingDirection)
            //         {
            //             //Top
            //             case 1:
            //                 break;
            //             //Left
            //             case 3:
            //                 break;
            //             //Right
            //             case 4:
            //                 break;
            //         }
            //     }
            //     //Left
            //     else if (roomSpawner.openingDirection == 3)
            //     {
            //         switch (openingDirection)
            //         {
            //             //Top
            //             case 1:
            //                 break;
            //             //Bottom
            //             case 2:
            //                 break;
            //             //Right
            //             case 4:
            //                 spawnedRoom = Instantiate(dungeon.GD, transform.position, Quaternion.identity);
            //                 dungeon.spawnedRooms.Add(spawnedRoom);
            //                 dungeon.roomSpawned++;
            //                 break;
            //         }
            //     }
            //     //Right
            //     else if (roomSpawner.openingDirection == 4)
            //     {
            //         switch (openingDirection)
            //         {
            //             //Top
            //             case 1:
            //                 spawnedRoom = Instantiate(dungeon.GB, transform.position, Quaternion.identity);
            //                 dungeon.spawnedRooms.Add(spawnedRoom);
            //                 dungeon.roomSpawned++;
            //                 break;
            //             //Bottom
            //             case 2:
            //                 break;
            //             //Left
            //             case 3:
            //                 spawnedRoom = Instantiate(dungeon.GD, transform.position, Quaternion.identity);
            //                 dungeon.spawnedRooms.Add(spawnedRoom);
            //                 dungeon.roomSpawned++;
            //                 break;
            //         }
            //     }
            //
            //     if (spawnedRoom != null)
            //     {
            //         Debug.Log(transform.parent.transform.parent.transform.name + " a fait spawn " +
            //                   spawnedRoom.transform.name);
            //     }
            //
            //     
            //     Destroy(gameObject);
            // }
            // hasSpawned = true;
            Destroy(gameObject);
        }
    }
}
    
