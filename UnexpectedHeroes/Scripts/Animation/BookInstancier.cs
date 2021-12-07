using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BookInstancier : MonoBehaviour
{
    [SerializeField] GameObject bookPrefabs;
    [SerializeField] GameObject[] bookTargets = new GameObject[3];
    [SerializeField] GameObject bookRepulsion;
    
    private void Start()
    {
        GameObject go = Instantiate<GameObject>(bookPrefabs);
        go.transform.position = bookTargets[0].transform.position;
        go.transform.rotation = bookTargets[0].transform.rotation;

        BookController bookController = go.GetComponent<BookController>();
        bookController.SetTarget(bookTargets);
        bookController.SetRepulsion(bookRepulsion);
    }

   
}
