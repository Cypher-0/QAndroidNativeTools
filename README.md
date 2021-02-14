# QNativeTools
Some files defining tools that can be used with Qt and android.

## Useful Files
	./CppAndroid
	|--- galleryResultReceiver.h
	|--- galleryResultReceiver.cpp
	|--- utils.h
	|--- utils.cpp

	./android
	|--- src
	|--- |--- QAndroidNativeTools
	|--- |--- |--- GalleryOpener.java

# Android precautions

## PRO file

Do not forget to add following code at the end of your `.pro` file :
```
android
{
	QT += androidextras

	ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
	DISTFILES += \
		android/src/QAndroidNativeTools/GalleryOpener.java
}
```
That means that your android related files are gonna be in the projectFolder/android.

You might (will) also need the `androidextras` package :
`QT	+= androidextras`

## Permissions

The file **./CppAndroid/utils.h** contains a function called `android::requirePermissions`.

It takes a list of permissions to ask and return a list of permissions denied.

There is an example of how to use it :
```cpp

//define permissions list to ask
QStringList appPermissions{"android.permission.READ_EXTERNAL_STORAGE","android.permission.WRITE_EXTERNAL_STORAGE"};

//get refused permissions
QStringList refusedPerm{android::requirePermissions(appPermissions)};
while(refusedPerm.size() != 0)//while there are some permissions denied
{
	QString errorMsg{tr("You need to grant following permissions :\n")};
    for(const auto& e : refusedPerm)
	    errorMsg += e+"\n";
	QMessageBox::warning(this,"Error",errorMsg);//list them and show it to the user
	refusedPerm = android::requirePermissions(appPermissions); //refresh them
}

```
> **_NOTE:_** Asking for permissions that were not defined in **manifest.xml** file might (will ?) lead to a runtime error.


## Errors

### Manifest.xml

Some errors can happen because of error in the current **manifest.xml** android file.

A solution might be to re-create one from qt built-in tool.





# Gallery opener


## How to use ?

The class `android::GalleryResultReceiver` defined in **galleryResultReceiver.h** will allow you to handle a request to native android gallery.

It has a signal **`handleActivityResult(QStringList list)`** that will be emitted as soon as a gallery request ends. The parameter correspond to a list of file path.

> **_NOTE:_** If the list is empty, that means that no file was selected.


So you will need to create an instance `android::GalleryResultReceiver` to be able to handle gallery requests.

When this instance is created, you will now be able to connect it's signal **`handleActivityResult(QStringList list)`** to the slots you want to use when some files are selected.

For example :
```cpp
auto m_galleryReceiver{std::make_unique<android::GalleryResultReceiver>()}

connect(m_galleryReceiver.get(),&android::GalleryResultReceiver::filesSelected,this,&MainWindow::onFilesSelected);
```

You will now be able to require the user to pick one or multiple images from a gallery app using the **`bool askImages(GalleryResultReceiver* receiver)`** function declared in **galleryResultReceiver.h**.

The parameter will be the object that will handle your request.
The return value allows you to know if the program achieved to request the gallery app to open (***true***) or not (***false***).

> **_NOTE:_** It doesn't say anything about user's choices.

For example :
```cpp
//if we can't open gallery
if(!android::askImages(m_galleryReceiver.get()))
{
	throw std::runtime_error{"Cannot open gallery"}; //throw runtime error
}
```

With this example, the **`MainWindow::onFilesSelected(QStringList filesList)`** will be called as soon as an user selected one or multiple files from the gallery app.