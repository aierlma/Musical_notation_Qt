#ifndef NOTE_H
#define NOTE_H


#include <QString>
#include <map>

class Note
{
    private:
        double frequency;
        double duration;
        QString orinote;
        QString notename;
        int notenum;
public:
    static double getFrequency(const QString& note);
    Note();
    Note(const QString & noteStr);
    
    Note(double frequency, double duration = 4.0);

    Note& operator=(const Note& note);
    ~Note() = default;
    double Frequency() const;
    double Duration() const;
    QString NoteName() const{
        return notename;
    }
    QString Ori() const{
        return orinote;
    }
    QString NoteNum() const{
        return QString::number(notenum);
    }
    Note operator+(const Note& other) const{
        double dur = 1.0/((1.0/this->duration)+(1.0/other.Duration()));
        return Note(this->frequency,dur);
    }
    void play() const;
    void changeDuration(double duration);
    void changePitch(QString pitch);

private:
    static std::map<QString, double> noteFrequencies;

};


#endif // NOTE_H
