#include <linux/init.h> // Macros utilizadas para marcar funciones, por ejemplo, __init __exit
#include <linux/module.h> // Cabecera del núcleo para cargar LKMs en el kernel
#include <linux/cdev.h> // Cabecera para el soporte del controlador de caracteres de Linux
#include <linux/device.h> // Cabecera para el soporte del Driver Model del kernel
#include <linux/kernel.h> // Contiene tipos, macros y funciones para el kernel
#include <linux/uaccess.h> // Necesario para la función de copia a usuario
#include <linux/fs.h> // Cabecera para el soporte del sistema de archivos de Linux

#define MAX_DEV 2 // Número de dispositivos a crear

static int mychardev_open(struct inode *inode, struct file *file); // Función de apertura para el controlador de dispositivos
static int mychardev_release(struct inode *inode, struct file *file); // Función de liberación del controlador de dispositivos
static long mychardev_ioctl(struct file *file, unsigned int cmd, unsigned long arg); // Función Ioctl para el controlador del dispositivo
static ssize_t mychardev_read(struct file *file, char __user *buf, size_t count, loff_t *offset); // Función de lectura para el controlador del dispositivo
static ssize_t mychardev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset); // Función de escritura para el controlador del dispositivo

static const struct file_operations mychardev_fops = { // Estructura de operaciones de archivo
        .owner = THIS_MODULE, // Dueño de la estructura
        .open = mychardev_open, // Función de apertura
        .release = mychardev_release, // Función de liberación
        .unlocked_ioctl = mychardev_ioctl, // Función Ioctl
        .read = mychardev_read, // Función de lectura
        .write = mychardev_write // Función de escritura
                // Ioctl is not supported in 32-bit ARM, so we use unlocked_ioctl
};

struct mychar_device_data {
    struct cdev cdev; // Estructura del dispositivo de caracteres
};

static int dev_major = 0; // Número mayor del controlador del dispositivo
static struct class *mychardev_class = NULL; // Estructura de clase para el controlador de dispositivo
static struct mychar_device_data mychardev_data[MAX_DEV]; // Estructura de datos del dispositivo

static int mychardev_uevent(struct device *dev, struct kobj_uevent_env *env) // Función Uevent para el controlador de dispositivo
{
    add_uevent_var(env, "DEVMODE=%#o", 0666); // Permisos de acceso
    return 0;
}

static int __init mychardev_init(void){ // Función de init para el controlador del dispositivo
    int err, i; // Variable de error
    dev_t dev; // Variable de número de dispositivo
    err = alloc_chrdev_region(&dev, 0, MAX_DEV, "mychardev"); // Asignación dinámica del número mayor
    dev_major = MAJOR(dev); // Extraer el número mayor
    mychardev_class = class_create(THIS_MODULE, "mychardev"); // Crear la estructura de la clase
    mychardev_class->dev_uevent = mychardev_uevent; // Asignando la función uevent

    for (i = 0; i < MAX_DEV; i++) { // Bucle para crear los archivos de dispositivos
        cdev_init(&mychardev_data[i].cdev, &mychardev_fops); // Inicializar la estructura del dispositivo de caracteres
        mychardev_data[i].cdev.owner = THIS_MODULE; // Asignar el propietario de la estructura de dispositivos de caracteres

        cdev_add(&mychardev_data[i].cdev, MKDEV(dev_major, i), 1); // Añadir el dispositivo de caracteres al kernel

        device_create(mychardev_class, NULL, MKDEV(dev_major, i), NULL, "mychardev-%d", i); // Crear el archivo de dispositivo
    }

    return 0;
}

static void __exit mychardev_exit(void) { // Función de salida del controlador de dispositivo
    int i; // Variable de bucle

    for (i = 0; i < MAX_DEV; i++) { // Bucle para eliminar los archivos del dispositivo
        device_destroy(mychardev_class, MKDEV(dev_major, i)); // Eliminar el archivo de dispositivo
    }

    class_unregister(mychardev_class); // Desregistrar la estructura de la clase
    class_destroy(mychardev_class); // Destruir la estructura de la clase

    unregister_chrdev_region(MKDEV(dev_major, 0), MINORMASK); // Desregistrar el número mayor
}

static int mychardev_open(struct inode *inode, struct file *file) { // Función de apertura del controlador de dispositivo
    printk("MYCHARDEV: Device open\n"); // Imprimir el mensaje en el log del kernel
    return 0;
}

static int mychardev_release(struct inode *inode, struct file *file){ // Función de liberación del controlador del dispositivo
    printk("MYCHARDEV: Device close\n"); // Imprimir el mensaje en el log del kernel
    return 0;
}

static long mychardev_ioctl(struct file *file, unsigned int cmd, unsigned long arg){ // Función Ioctl para el controlador del dispositivo
    printk("MYCHARDEV: Device ioctl\n"); // Imprimir el mensaje en el log del kernel
    return 0;
}

static ssize_t mychardev_read(struct file *file, char __user *buf, size_t count, loff_t *offset){ // Función de lectura para el controlador del dispositivo

    uint8_t *data = "Hello from the kernel world!\n";
    size_t datalen = strlen(data);

    size_t datalen = strlen(data); // Longitud de los datos

    printk("Leyendo dispositivo: %d\n", MINOR(file->f_path.dentry->d_inode->i_rdev)); // Imprimir el mensaje en el log del kernel

    if (count > datalen) { // Comprobar si el tamaño del buffer es mayor que la longitud de los datos
        count = datalen; // Establecer el tamaño del buffer a la longitud de los datos
    }

    if (copy_to_user(buf, data, count)) { // Copiar los datos al espacio de usuario
        return -EFAULT; // Devuelve el error
    }

    return count; // Devuelve el número de bytes leídos
}

static ssize_t mychardev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset){ // Función de escritura para el controlador del dispositivo
    size_t maxdatalen = 30, ncopied; // Longitud máxima de los datos y número de bytes copiados variables
    uint8_t databuf[maxdatalen]; // Buffer de datos

    printk("Escribiendo dispositivo: %d\n", MINOR(file->f_path.dentry->d_inode->i_rdev)); // Imprimir el mensaje en el log del kernel

    if (count < maxdatalen) { // Comprobar si el tamaño del buffer es menor que la longitud máxima de los datos
        maxdatalen = count; // Establecer la longitud máxima de los datos al tamaño del buffer
    }

    ncopied = copy_from_user(databuf, buf, maxdatalen); // Copiar los datos del espacio de usuario al buffer del espacio del núcleo y obtener el número de bytes copiados

    if (ncopied == 0) { // Comprobar si el número de bytes copiados es cero
        printk("Copied %zd bytes from the user\n", maxdatalen); // Imprimir el mensaje en el log del kernel
    } else { // Si el número de bytes copiados no es cero
        printk("No se han podido copiar %zd bytes del usuario\n", ncopied); // Imprimir el mensaje en el log del kernel
    }

    databuf[maxdatalen] = 0; // Poner a cero el último byte del buffer

    printk("Datos del usuario: %s\n", databuf); // Imprimir el mensaje en el registro del núcleo

    return count; // Devuelve el número de bytes escritos
}

MODULE_LICENSE("GPL"); // Establecer la licencia del módulo
MODULE_AUTHOR("Oleg Kutkov <elenbert@gmail.com>"); // Establecer el autor del módulo

module_init(mychardev_init); // Asignar la función init al módulo
module_exit(mychardev_exit); // Asignar la función de salida al módulo

/*
 este es el código de la aplicación de espacio de usuario que se utilizará para probar el controlador de dispositivo anterior

-descripción del módulo:
Este es un simple controlador de dispositivo de caracteres para el kernel de Linux. Crea un archivo de dispositivo en el directorio /dev y permite al
usuario leer y escribir en él.

El archivo de dispositivo se llama mychardev-0, mychardev-1, etc. dependiendo del número de dispositivos creados. El número máximo de dispositivos está definido por
la macro MAX_DEV.
*/