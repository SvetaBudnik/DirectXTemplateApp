using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Timers;
using Timer = System.Timers.Timer;

namespace TemplateApp
{
    public partial class DxControl : UserControl
    {
        static Timer timer;
        float angle = (float)Math.PI / 32;
        float plusangle = (float)Math.PI / 32;

        bool initialized = false;
        bool enablePaint;

        public void set_alg(float alg)
        {
            plusangle = alg;
        }

        private void SetTimer()
        {
            // Create a timer with a two second interval.
            timer = new System.Timers.Timer(100);
            // Hook up the Elapsed event for the timer. 
            timer.Elapsed += OnTimedEvent;
            timer.AutoReset = true;
            timer.Enabled = true;
        }

        private void OnTimedEvent(object sender, ElapsedEventArgs e)
        {
            angle += plusangle;
            EnablePaint = true;
            Invalidate();
        }

        public bool EnablePaint
        {
            get { return enablePaint; }
            set
            {
                enablePaint = value;
                if (enablePaint)
                {
                    if (!initialized)
                    {
                        Invalidate();
                    }
                }
            }
        }

        public DxControl()
        {
            SetTimer();
            InitializeComponent();
        }

        protected override void OnHandleCreated(EventArgs e)
        {
            base.OnHandleCreated(e);

            if (!initialized)
            {
                try
                {
                    if (Handle == IntPtr.Zero)
                    {
                        throw new InvalidOperationException("Handle is not initialized");
                    }

                    DllImportFunctions.InitDirectX((int)Handle);
                    DllImportFunctions.PrepareScene((int)Handle, Width, Height);
                    initialized = true;
                }
                catch (Exception ex)
                {
                    MessageBox.Show($"Error initializing DirectX: {ex.Message}");
                }
            }
        }

        protected override void OnPaintBackground(PaintEventArgs pevent) { }

        private void DxControl_Paint(object sender, PaintEventArgs e)
        {
            if (initialized)
            {
                DllImportFunctions.RenderScene((int)Handle, angle);
            }
        }

        private void DxControl_SizeChanged(object sender, EventArgs e)
        {
            if (initialized) EnablePaint = true;
            Invalidate();
        }
    }

    public class DllImportFunctions
    {
        [DllImport("DirectXCppCode.Dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "InitDirectX", SetLastError = true)]
        public static extern void InitDirectX(int hwnd);

        [DllImport("DirectXCppCode.Dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "PrepareScene", SetLastError = true)]
        public static extern void PrepareScene(int hwnd, int w, int h);

        [DllImport("DirectXCppCode.Dll", CallingConvention = CallingConvention.Cdecl, EntryPoint = "RenderScene", SetLastError = true)]
        public static extern void RenderScene(int hwnd, float angle);

        // DLL libraries used to manage hotkeys
    }
}