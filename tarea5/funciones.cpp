#include "funciones.h"

/*Funcion que valida si los argumento ingresados  se encuentran*/
bool argumento_correcto(int indice , char *argv[]){
  try{ //Se intentará cargar un archivo
    workbook probando; //instancia de objeto que aloja el xlsx
    probando.load(argv[indice]); //carga del xlsx en instancia de prueba
  }
  catch (xlnt::exception) //SI NO SE PUEDE ABRIR EL ARCHIVO
  {
     return false;
   }
   return true;
}

//Cuenta la cantidad de filas que hay los archivos mandados por argumento.
int cantidadFilasPorArchivo(int argc, char *argv[]){
    int condicional=0; //Variable que determina cuantos archivos pueden ser abiertos
    if(argc > 1){ //Verifica la cantidad de argumentos a momento de ejecutar
      cout << "Cantidad de archivos ingresados: " << argc-1 << endl << endl;
      //iteracion desde el el 2° objeto (1° es nombre del .cpp)
      for(int archivos = 1; archivos < argc; archivos++){
        cout << "Nombre archivo: " << argv[archivos] << endl;

        if (argumento_correcto(archivos, argv)) {
          workbook xlsCursos; //instancia de objeto que aloja el xlsx
          xlsCursos.load(argv[archivos]); //carga del xlsx
          auto sheetCursos = xlsCursos.active_sheet(); //trabajando con la 1° pestaña del archivo

          int contadorFilas = 0;
          for (auto row : sheetCursos.rows(false)) //seudoForEach que recorrelas filas.
          {
            contadorFilas++;
          }

          cout << "Cantidad de filas: " << contadorFilas << endl << endl;

          condicional++;
        }
        else{
          cout << "Lo sentimos, el archivo " <<argv[archivos] <<" no puede ser abierto."<< endl << endl;
        }

      }
    } else {
      cout << "No se ingresaron archivos .xlsx" << endl;
      return condicional;
    }

    if (condicional==3) { //Si todos los archivos pueden ser abiertos, entonces el programa funcionará correctamente
      return 1;
    }else{
      return 0;   //Si algún archivo no puede ser abierto, el programa no seguirá ejecutandose.
    }
}


//Muestra por pantalla el contenido de las primeras N filas
//de un vector bidimensional (Filas/Columnas)
void imprimirVectorNPrimeros(vector<vector<string>> vector, int nPrimeros){
    for (int fila = 1; fila <= nPrimeros; fila++) //Itera por fila hasta N
    {
      cout << "| ";
        for (int columna = 0; columna < vector.at(fila).size(); columna++) //itera por columna
        {
            //Imprime lo que esta en la fila y columna indicada
            cout << vector.at(fila).at(columna) << " | ";
        }
        cout << endl;
    }
}

//Muestra por pantalla una lista de los profesores desde un vector
void imprimeProfesores(vector<vector<string>> profes){
    for (int fila = 1; fila < profes.size(); fila++) //Itera por fila
    {
        cout << fila << ".- ";
        for (int columna = 0; columna < 3; columna++) //itera por columna
        {
            //Imprime lo que esta en la fila y columna indicada
            cout << profes.at(fila).at(columna) << " ";
        }
        cout << endl;
    }
}

//Cuenta la cantidad de asignarutas de cada profesor y las muestra por pantalla
void cantidadAsignaturasPorProfesor(int argc, char *argv[]){
    std::cout << "Calculando cantidad de ASIGNATURAS POR PROFESOR" << endl<<endl;
    workbook xlsCursos; //instancia de objeto que aloja el xlsx
    xlsCursos.load(argv[1]); //carga del xlsx
    auto sheetCursos = xlsCursos.active_sheet(); //seleccionar 1° pestaña

    workbook xlsDocentes; //instancia de objeto que aloja el xlsx
    xlsDocentes.load(argv[2]); //carga xlsx
    auto sheetDocentes = xlsDocentes.active_sheet(); //seleccionar 1° pestaña

    vector< vector<string> > vectorCursos; //Vector de vectores de los cursos
    vector< vector<string> > vectorDocentes; //Vector de vectores de los docentes

    for (auto row : sheetCursos.rows(false)) //se recorre el xlsx de Cursos por fila
    {
        vector<string> filaCursos; //vector auxiliar para almacenar datos del xlsx
        for (auto cell : row) //se recorre las filas celda por celda
        {
            //push_back del contenido de las celdas al vector auxiliar
            filaCursos.push_back(cell.to_string());
        }
        vectorCursos.push_back(filaCursos); //push_back del vector auxiliar al vector de vectores
    }

    //igual que el anterior solo que con los docentes
    for (auto row : sheetDocentes.rows(false))
    {
        vector<string> filaDocentes;
        for (auto cell : row)
        {
            filaDocentes.push_back(cell.to_string());
        }
        vectorDocentes.push_back(filaDocentes);
    }
    int contadorTotalAsignaturas = 0;
    //Se itera los profesores que hay en el vector de vectores de profesores
    for (int profesores = 1; profesores < vectorDocentes.size(); profesores++)
    {
        int contadorAsignaturasPorProfesor = 0;
        //se itera las asignaturas que hay en el vector de vectores de asignaturas
        for (int asignaturas = 1; asignaturas < vectorCursos.size(); asignaturas++){
            //verificacion si el id del profesor que esta relacionado en
            //las asignaturas concuerda con el id del profesor
            if(vectorCursos.at(asignaturas).at(2) == vectorDocentes.at(profesores).at(0)){
                contadorAsignaturasPorProfesor++; //si coincide se aumenta el contador
                contadorTotalAsignaturas++; //aumenta la cantidad de asignaturas
            }
        }
        //Mostrar por pantalla el nombre del profesor y su cantidad de asignarutas
        cout << profesores << ".- " << vectorDocentes.at(profesores).at(1) << " "
        << vectorDocentes.at(profesores).at(2) << " tiene: " <<
        contadorAsignaturasPorProfesor << " asignaturas." << endl;

    }
    //imprime el total de asignaturas contadas
    cout << endl << "Total Asignaturas: " << contadorTotalAsignaturas << endl;

}


/* Funcion que permite validar el ingreso correcto de los argumentos */
void validarArgumentos(int argc, char *argv[]){
  int continuar=0; //Variable que determinará si es seguro continuar con la ejecución.

  if (argc-1 != 3){
      cout << endl << "Argumentos Invalidos" << endl;
  }
  else{
      cout << endl << "Argumentos Validos" << endl;
      continuar=cantidadFilasPorArchivo(argc, argv);
      if (continuar==1) {
        cantidadAsignaturasPorProfesor(argc, argv);
      }
      else{
        cout<<"Se reportan problemas con algunos archivos, favor verificar si fueron bien ingresados o si existen." <<endl;
      }
    }
}
