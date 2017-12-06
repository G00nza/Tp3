#ifndef TP3_IMPL_H
#define TP3_IMPL_H

#include "tp3.h"

#include <limits>
#include <algorithm>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
/// EJERCICIO 1
template <typename iterator>
typename iterator::value_type maximoIter(iterator desde, iterator hasta) {
    iterator res = desde;
    for (auto it = desde; it != hasta; ++it) {
        if (int(*it) > int(*res))
            res = it;
    }

    return *res;
}

template <typename iterator>
typename iterator::value_type minIter(iterator desde, iterator hasta) {
    iterator res = desde;
    for (auto it = desde; it != hasta; ++it) {
        if (int(*it) < int(*res))
            res = it;
    }

    return *res;
}

template <typename iterator, typename bucket>
bucket sortAux(typename bucket::iterator desde, typename bucket::iterator hasta) {
    bucket res;
    for (auto it = desde; it != hasta; ++it){
        auto insert = *it;
        for (auto min = it; min != hasta; ++min ){
            if (min < insert){
                auto aux = min;
                min = insert;
                insert = aux;
            }
        }
        res.insert(desde, insert);
    }
    return res;
}




template <typename iterator, typename bucket>
vector<bucket> generar_buckets(iterator input_begin, iterator input_end) {

    int max = int(maximoIter(input_begin, input_end));
    int min = int(minIter(input_begin, input_end));
    int separador = ((max - min)+1);

    bucket aux1;
    vector<bucket> v;
    v.resize(separador, aux1);
    assert(v.size() == separador);

    for(auto it = input_begin; it != input_end; ++it){
        auto aux = *it;
        v[int(aux)-min].insert(v[int(aux)-min].begin(), aux);
        /*if (v[int(aux)-min].size() == 0){
            v[int(aux)-min].insert(v[int(aux)-min].begin(), aux);
        }
        for (auto itV = v[int(aux)-min].begin(); itV != v[int(aux)-min].end(); ++itV ){
            auto aux2 = *itV;
            if (aux2 > aux){
                v[int(aux)-min].insert(itV, aux);
            }
        }*/
    }
    for (int i = 0; i < v.size() ; i++){
        if (v[i].size() > 0){
            bucket res;
            for (auto it = v[i].begin(); it != v[i].end(); ++it){
                auto insert = *it;
                for (auto minIt = it; minIt != v[i].end(); ++minIt ){
                    if (*minIt < insert){
                        insert = *minIt;
                    }
                }
                res.insert(res.end(), insert);
            }
            v[i] = res;
        }
    }


    return v;
}

template <typename bucket>
vector<typename bucket::value_type> aplanar_buckets(const std::vector<bucket> & B) {
    vector<typename bucket::value_type> res;

    for (int i = 0; i < B.size();++i){
        for (auto it : B[i]) {
            res.push_back(it);
        }
    }

    return res;
}

///////////////////////////////////////////////////////////////////////////////
/// EJERCICIO 2
////

/// busqueda binaria /// O(log n)no funciona de ninguna manera (recursiva o con while)LPM!!!!
/*inline bool perteneceBinario(const fajo& falsos, billete actual, ulong i, ulong j){
/ //   bool res = false;
//    ulong aux = (i + j)/2;
//    std::cout<<"i: " << i <<" j: "<< j << std::endl;
//    std::cout<< "(i+j)/2: "<< aux << std::endl;
    ulong i = 0;
    ulong j = falsos.size()-1;
    while (j-i > 1){
        ulong aux = (i/2 + j/2);
//        std::cout<<"i: " << i <<" j: "<< j << std::endl;
//        std::cout<< "(i+j)/2: "<< aux << std::endl;
        if (falsos[aux] == actual){
//            std::cout<< "Encontrado "<< aux << std::endl;
            return true;
        } else{
            if (falsos[aux] < actual){
                i = aux;
            } else {
                j = aux;
            }
        }
    }
//    std::cout<< "en i "<< (falsos[i] == actual) << std::endl;
//    std::cout<< "en j "<< (falsos[j] == actual) << std::endl;
    return falsos[i] == actual || falsos[j] == actual || falsos[i+1] == actual ;
*/
/*
    bool res = false;
    if (i == j){
        return falsos[i] == actual;
    } else if (falsos[(i + j) / 2] == actual) {
        return true;
    } else {
        if (falsos[(i + j) / 2] < actual) {
            res = perteneceBinario(falsos, actual, (j)/2, j);
        } else {
            res = perteneceBinario(falsos, actual, i, ((j)/2)-1);
        }
    }
    return res;
}*/


fajo ordenar_por_probabilidad(const fajo& falsos_conocidos, const fajo& a_ordenar) {
    fajo falsosAOrdenar;
    fajo correctosAOrdenar;
    falsosAOrdenar.reserve(a_ordenar.size());
    correctosAOrdenar.reserve(a_ordenar.size());
    assert(falsosAOrdenar.capacity() == a_ordenar.size());
    assert(correctosAOrdenar.capacity() == a_ordenar.size());

    std::vector <int > contador;
    contador.reserve(10000);
    for (auto it : falsos_conocidos){
        contador[int(it)]++;
    }
    fajo falsosPropios;
    falsosPropios.insert(falsosPropios.begin(), falsos_conocidos.begin(), falsos_conocidos.end());
    std::sort(falsosPropios.begin(), falsosPropios.end());
    assert(falsosPropios.capacity() == falsos_conocidos.size());

    for (int i = 0; i < a_ordenar.size() ; ++i) {
        if (std::binary_search(falsosPropios.begin(), falsosPropios.end(), a_ordenar[i]) ){ //contador[int (a_ordenar[i])].count(a_ordenar[i])==1
/*  ninguno de estos mejora la complejidad cte a 0.45 +/- .05
 *  // std::binary_search(falsos_conocidos.begin(),falsos_conocidos.end(), a_ordenar[i]) //no busca bien
 *  // falsosMap.count(a_ordenar[i])==1 // igual que 3
 *  // vectorCounting[a_ordenar[i].numero_de_serie]==1
 *  // falsosMap.find(a_ordenar[i])!=falsosMap.end() //igual que 2
 *  // perteneceBinario(falsos_conocidos,a_ordenar[i], 0, falsos_conocidos.size()-1) //no busca bien
 */
            falsosAOrdenar.push_back(a_ordenar[i]);
        } else {
            correctosAOrdenar.push_back(billete(a_ordenar[i].numero_de_serie, contador[int (a_ordenar[i])]));
        }
    }
    std::sort(falsosAOrdenar.begin(), falsosAOrdenar.end());
    std::sort(correctosAOrdenar.rbegin(), correctosAOrdenar.rend());
    correctosAOrdenar.insert(correctosAOrdenar.begin(), falsosAOrdenar.rbegin(), falsosAOrdenar.rend());

    return correctosAOrdenar;
}
/// total ejecucion = O(n * log(n) + n + p * log(p * n) + p )
/// O (n * (log (n) + 1) + p * (log (p * n) + 1) )
/// O (n * log (n) + p * log (p * n) )

/*mail a la catedra(?)
 * buenos dias, resolviendo el ejercicio 2 del TP 3 no logro pasar uno de los test de complejidad, este es el mensaje de los test:

T0: 103731329
T1: 3442422225
T0/T1: 0.0301332

T0: 5926345
T1: 49977064
T0/T1: 0.118581

T0: 905341535
T1: 1819166708
T0/T1: 0.497668
/home/gsaenz/TP 3/tests/test-ordenar-billetes.cpp:207: Failure
Expected: (d) <= (0.25), actual: 0.497668 vs 0.25

analizando la complejidad de mi codigo,  cumplo con lo pedido en el marco teorico y no logro encontrar donde esta el codigo de mas
 // en realidad se donde esta, pero por la condicion inline no puedo armar funciones auxiliares



*/

///////////////////////////////////////////////////////////////////////////////
/// EJERCICIO 3
////
Matriz suma(const Matriz& M1, const Matriz& M2){

    Matriz C = M1;

    for(int i=0; i < M1.size(); i++){
        for(int j=0; j < M1.size(); j++){

            C[i][j] = M1[i][j] + M2[i][j];
//            std::cerr << "valor en m1 " << M1[i][j] << std::endl;
//            std::cerr << "valor en m2 " << M2[i][j] << std::endl;
//            std::cerr << "valor en C " << i << " " << j << " " << C[i][j] << std::endl;
        }
    }

    return C;
}


Matriz resta(const Matriz& M1, const Matriz& M2){
    Matriz C = M1;

    for(int i=0; i < M1.size(); i++){
        for(int j=0; j < M1.size(); j++){
            C[i][j] = M1[i][j] - M2[i][j];
        }
    }

    return C;
}

 Matriz multiplicar_strassen(const Matriz& A, const Matriz& B, int K) {

    Matriz C(A.size(), std::vector<double>(A.size(),0.0) );
    if (A.size() <= K){
        return multiplicar(A,B);
    } else{
        std::vector<double > aux1;
        aux1.resize(A.size()/2, 0);

        Matriz A1(A.size()/2, aux1);
        Matriz A2(A.size()/2, std::vector<double>(A.size()/2,0.0) );
        Matriz A3(A.size()/2, std::vector<double>(A.size()/2,0.0) );
        Matriz A4(A.size()/2, std::vector<double>(A.size()/2,0.0) );
        for (int i = 0; i < A.size()/2 ; ++i) {
            for (int j = 0; j < A.size()/2 ; ++j) {
                A1[i][j] = A[i][j];   //std::cout << A[i][j] << std::endl;
            }
        }
        for (int i = 0; i < A.size()/2 ; ++i) {
            for (int j = A.size()/2; j < A.size(); ++j) {
                A2[i][j-A.size()/2] = A[i][j];
            }
        }
        for (int i = A.size()/2; i < A.size(); ++i) {
            for (int j = 0; j < A.size()/2 ; ++j) {
                A3[i-A.size()/2][j] = A[i][j];

            }
        }
        for (int i = A.size()/2; i < A.size() ; ++i) {
            for (int j = A.size()/2; j < A.size() ; ++j) {
                A4[i-A.size()/2][j-A.size()/2] = A[i][j];
            }
        }
        Matriz B1(A.size()/2, vector<double>(A.size()/2,0.0) );
        Matriz B2(A.size()/2, vector<double>(A.size()/2,0.0) );
        Matriz B3(A.size()/2, vector<double>(A.size()/2,0.0) );
        Matriz B4(A.size()/2, vector<double>(A.size()/2,0.0) );

        for (int i = 0; i < B.size()/2 ; ++i) {
            for (int j = 0; j < B.size()/2 ; ++j) {
                B1[i][j] = B[i][j];
            }
        }
        for (int i = 0; i < B.size()/2 ; ++i) {
            for (int j = B.size()/2; j < B.size(); ++j) {
                B2[i][j-B.size()/2] = B[i][j];
                //std::cout << B[i][j] << std::endl;
            }
        }
        for (int i = B.size()/2; i < B.size(); ++i) {
            for (int j = 0; j < B.size()/2 ; ++j) {
                B3[i-B.size()/2][j] = B[i][j];
            }
        }
        for (int i = B.size()/2; i < B.size() ; ++i) {
            for (int j = B.size()/2; j < B.size() ; ++j) {
                B4[i-B.size()/2][j-B.size()/2] = B[i][j];
            }
        }
        Matriz auxj = (suma(suma(A1,A2),suma(A3,A4)));
        Matriz auxK = (suma(suma(B1,B2),suma(B3,B4)));

        Matriz M1 = multiplicar_strassen(suma(A1, A4), suma(B1,B4), K);
        Matriz M2 = multiplicar_strassen(suma(A3, A4), B1, K);
        Matriz M3 = multiplicar_strassen( A1, resta(B2,B4), K);
        Matriz M4 = multiplicar_strassen( A4, resta(B3,B1), K);
        Matriz M5 = multiplicar_strassen(suma(A1, A2), B4, K);
        Matriz M6 = multiplicar_strassen(resta(A3, A1), suma(B1,B2), K);
        Matriz M7 = multiplicar_strassen(resta(A2, A4), suma(B3,B4), K);

        Matriz C1 = resta(suma(M1,suma(M4, M7)), M5);
        Matriz C2 = suma(M3,M5);
        Matriz C3 = suma(M2, M4);
        Matriz C4 = resta(suma(M1, suma(M3, M6)),M2);

        //C1 = resta(suma(M1,suma(M4, M7)), M5);
        //C2 = suma(M3,M5);
        //C3 = suma(M2, M4);
        //C4 = resta(suma(M1, suma(M3, M6)),M2);
        for (int i = 0; i < C.size()/2 ; ++i) {
            for (int j = 0; j < C.size()/2 ; ++j) {
                C[i][j] = C1[i][j];
                //std::cout << C[i][j] << std::endl;
            }
        }
        for (int i = 0; i < C.size()/2 ; ++i) {
            for (int j = C.size()/2; j < C.size(); ++j) {
                C[i][j] = C2[i][j-C.size()/2];
            }
        }
        for (int i = C.size()/2; i < C.size(); ++i) {
            for (int j = 0; j < C.size()/2 ; ++j) {
                C[i][j] = C3[i-C.size()/2][j];
            }
        }
        for (int i = C.size()/2; i < C.size() ; ++i) {
            for (int j = C.size()/2; j < C.size() ; ++j) {
                C[i][j] = C4[i-C.size()/2][j-C.size()/2];
            }
        }

    }

    return C;
}




#endif // TP3_IMPL_H
