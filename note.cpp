#include "note.h"
#include <QStringList>
#include <SFML/Audio.hpp>
#include <QString>
#include <QRegularExpression>


Note::Note():
    frequency(0.0),
    duration(4.0)
{
    
}

int extractInteger(const QString& str) {

    QRegularExpression re("\\d+");  // Regular expression to match digits
    QRegularExpressionMatch match = re.match(str);  // Apply the regular expression to the string

    int num = -1;
    if (match.hasMatch()) {
        QString numStr = match.captured(0);  // Get the matched string
        bool ok;
        num = numStr.toInt(&ok);  // Convert the string to an integer
        return num;
        if (!ok) {
            // Handle error
            return -1;
        }
    }
}

Note::Note(const QString& noteStr)
{
    // check if noteStr contain +
    if (noteStr.contains('+')) {
        orinote = noteStr;
        QStringList parts = noteStr.split('+');
        // parts will have many parts, so I need to add them together
        QString *note = new QString[parts.size()];
        double *dur = new double[parts.size()];
        for (int i = 0; i < parts.size(); i++) {
            QStringList partss = parts[i].split('-'); // partss will have 1 or 2 parts (note and duration)
            note[i] = partss[0];
            dur[i] = partss.size() > 1 ? partss[1].toDouble() : 4.0; // Default to quarter note
        }
        double durSum = 0.0;
        for (int i = 0; i < parts.size(); i++) {
            durSum += 1.0/dur[i];
        }
        double durt = 1.0/durSum;
        frequency = getFrequency(note[0]);
        notename = note[0];
        notenum = extractInteger(note[0]);
        duration = durt;
        delete [] note;
        delete [] dur;
    } // check if note 是附点音符,即末尾是 '.'
    else if (noteStr.endsWith('.')){
        orinote = noteStr;
        QStringList partss = noteStr.split('.');
        QStringList parts = partss[0].split('-');
        QString note = parts[0];
        duration = parts.size() > 1 ? parts[1].toDouble() : 4.0; // Default to quarter note
        frequency = getFrequency(note);
        notename = note;
        notenum = extractInteger(note);
        duration /=  1.5;
    }else {
    orinote = noteStr;
    QStringList parts = noteStr.split('-');
    QString note = parts[0];
    if (note == "0") {
        frequency = 0.0;
        notename = note;
        notenum = extractInteger(note[0]);
        duration = parts.size() > 1 ? parts[1].toDouble() : 4.0; // Default to quarter note
    } else {
    duration = parts.size() > 1 ? parts[1].toDouble() : 4.0; // Default to quarter note
    frequency = getFrequency(note);
    notename = note;
    notenum = extractInteger(note);
    }
    }
}


Note::Note(double frequency, double duration):
    frequency(frequency),
    duration(duration)
{
    
}


Note& Note::operator=(const Note& note)
{
    if (this != &note)
    {
        frequency = note.frequency;
        duration = note.duration;
        notename = note.notename;
        notenum = note.notenum;
        orinote = note.orinote;
    }
    return *this;
}

double Note::Frequency() const
{
    return frequency;
}

double Note::Duration() const
{
    return duration;
}

std::map<QString, double> Note::noteFrequencies = {
    {"_1", 130.813}, // C3
    {"_#1", 138.591}, // C#3
    {"_b2", 138.591}, // Db3
    {"_2", 146.832}, // D3
    {"_#2", 155.563}, // D#3
    {"_b3", 155.563}, // Eb3
    {"_3", 164.814}, // E3
    {"_4", 174.614}, // F3
    {"_#4", 184.997}, // F#3
    {"_b5", 184.997}, // Bb3
    {"_5", 195.998}, // G3
    {"_#5", 207.652}, // G#3
    {"_b6", 207.652},// Ab3
    {"_6", 220.000}, // A3
    {"_#6", 233.082}, // A#3
    {"_b7", 233.082}, // Bb3
    {"_7", 246.942}, // B3
    {"1", 261.626}, // C4
    {"#1", 277.183}, // C#4
    {"b2", 277.183}, // Db4
    {"2", 293.665}, // D4
    {"#2", 311.127}, // D#4
    {"b3", 311.127}, // Eb4
    {"3", 329.628}, // E4
    {"4", 349.228}, // F4
    {"#4", 369.994}, // F#4
    {"b5", 369.994}, // Bb4
    {"5", 391.995}, // G4
    {"#5", 415.305}, // G#4
    {"b6", 415.305}, // Ab4
    {"6", 440.000}, // A4
    {"#6", 466.164}, // A#4
    {"b7", 466.164}, // Bb4
    {"7", 493.883}, // B4
    {"^1", 523.251}, // C5
    {"^#1", 554.365}, // C#5
    {"^b2", 554.365}, // Db5
    {"^2", 587.330}, // D5
    {"^#2", 622.254}, // D#5
    {"^b3", 622.254}, // Eb5
    {"^3", 659.255}, // E5
    {"^4", 698.456}, // F5
    {"^#4", 739.989}, // F#5
    {"^b5", 739.989}, // Bb5
    {"^5", 783.991}, // G5
    {"^#5", 830.609}, // G#5
    {"^b6", 830.609}, // Ab5
    {"^6", 880.000}, // A5
    {"^#6", 932.328}, // A#5
    {"^b7", 932.328}, // A#5
    {"^7", 987.767}, // B5
};


double Note::getFrequency(const QString& note)
{
    if (noteFrequencies.find(note) != noteFrequencies.end())
        return noteFrequencies[note];
    else
        return 0.0; // Note not found, return 0 frequency or handle it accordingly
}

const double TIME = 2400.0; // how many seconds in a 小节

static void playNote(double frequency, double duration)
{
    // Create a new sound buffer
    sf::SoundBuffer buffer;

    // Create a array to store the audio data
    const int sampleRate = 44100;
    const int seconds = 5;  // Change this to change the length of the sound
    sf::Int16 raw[sampleRate * seconds];

    // Generate a sine wave
    for (int i = 0; i < sampleRate * seconds; ++i)
        raw[i] = 30000 * std::sin(2 * 3.14159265 * frequency * i / sampleRate);

    // Load the buffer with the audio data
    buffer.loadFromSamples(raw, sampleRate * seconds, 1, sampleRate);

    // Create a sound and set its buffer
    sf::Sound sound;
    sound.setBuffer(buffer);

    // Play the sound
    sound.play();

    // Wait for the duration of the note
    sf::sleep(sf::milliseconds(int(TIME / duration)));
}

static void playNote(const Note * n)
{
    playNote (n->Frequency(),n->Duration());
}

void Note::play() const
{
    if (frequency == 0.0){
        sf::sleep(sf::milliseconds(int(TIME / duration)));
        return;
    }else
    {
        playNote (this);
        return;}
    
}

void Note::changeDuration(double newDuration)
{
    duration = newDuration;
}

void Note::changePitch(QString newPitch)
{
    double dur = this->duration;
    *this = Note(newPitch);
    this->changeDuration(dur);
}