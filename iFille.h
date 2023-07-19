#ifndef IFILLE_H
#define IFILLE_H
#include<QtWidgets>

namespace ap {
    struct iSave{
        virtual void save()=0;
    };
    struct iSave_as{
        virtual void save_as()=0;
    };

    struct BaseDataFile{
        QByteArray text;
        BaseDataFile();
    };

    struct DataFileOpen: public BaseDataFile{
        QString path_file;
        DataFileOpen(const QString & _path_file);
    };

    struct DataFileCreate: public BaseDataFile{
        DataFileCreate();
    };

    struct IFile:public iSave, public iSave_as{
        std::unique_ptr<BaseDataFile> dataFile;
        QByteArray & text;
        IFile(BaseDataFile * baseDataFile);
        virtual ~IFile() = default;
        void save_as() override;
    };

    class SaveOpenFile:public IFile{
    public:
        SaveOpenFile(QString path_file);
        ~SaveOpenFile()=default;
        void save() override;
    };

    struct SaveFileCreate: public IFile{
        SaveFileCreate();
        ~SaveFileCreate()=default;
        void save() override;
    };
}
#endif // IFILLE_H
