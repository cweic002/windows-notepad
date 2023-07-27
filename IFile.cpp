#include "IFille.h"

ap::BaseDataFile::BaseDataFile():text(){}

ap::DataFileOpen::DataFileOpen(const QString & _path_file):BaseDataFile(),path_file(_path_file){}

ap::DataFileCreate::DataFileCreate():BaseDataFile(){}

ap::IFile::IFile(BaseDataFile * baseDataFile):dataFile(baseDataFile),text(dataFile->text){}

void ap::IFile::save_as(){
    QString _path_file = QFileDialog::getSaveFileName(nullptr, QFileDialog::tr("сохранить файл"), QDir::currentPath(), QFileDialog::tr("*.txt"));
    if (_path_file.isEmpty() == false)
    {
        QFile file(_path_file);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;
        file.write(dataFile->text);
        file.close();
        new(this) SaveOpenFile(_path_file);
    }
}

ap::SaveOpenFile::SaveOpenFile(QString path_file):IFile(new ap::DataFileOpen(path_file)){};
void ap::SaveOpenFile::save() {
    union {
        BaseDataFile * baseDataFile;
        DataFileOpen * dataFileOpen;
    };
    baseDataFile=&*(this->dataFile);
    QFile file(dataFileOpen->path_file);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
       return;
    file.write(text);
    file.close();
}

ap::SaveFileCreate::SaveFileCreate():IFile(new DataFileCreate()){};
void ap::SaveFileCreate::save(){
   this->save_as();
}



